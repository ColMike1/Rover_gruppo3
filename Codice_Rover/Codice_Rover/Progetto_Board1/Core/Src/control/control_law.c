/*
 * control_law.c
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */
#include <actuation/actuation_step.h>
#include <encoder/encoder_step.h>
#include "control/control_law.h"

/* ===== Struttura PI ===== */
typedef struct {
    float Kp;
    float Ki;
    float e_prev;
    float u_prev;
    float out_limit;
} PI_Controller;

/* ===== Parametri ===== */
static const float Ts = 0.010f;   /* 10 ms */

/* ===== Controller ===== */
/* 1. Master: Controlla la velocità media di tutto il rover */
/*
 * static PI_Controller pi_global = {0.061f, 1.247f, 0.0f, 0.0f, 12.0f}; // Controllore troppo veloce(0.2 sec)
 * static PI_Controller pi_global = {0.00235f, 0.0483f, 0.0f, 0.0f, 12.0f}; // Controllore più lento(5.44 sec)
 * static PI_Controller pi_global = {0.003156f, 0.06485f, 0.0f, 0.0f, 12.0f}; // Controllore lento(5.44 sec)
 *
 */
static PI_Controller pi_global = {0.013f, 0.267f, 0.0f, 0.0f, 12.0f}; // Controllore finale(1 sec)

/* 2. Sync Assi: Bilancia la velocità tra asse anteriore e posteriore */
static PI_Controller pi_sync_asse = {0.005f,  0.189f,  0.0f, 0.0f, 2.4f};

/* 3. Differenziale Posteriore: Bilancia DX e SX dietro */
static PI_Controller pi_diff_p = {0.01f, 0.25f, 0.0f, 0.0f, 6.0f};

/* 4. Differenziale Anteriore: Bilancia DX e SX davanti */
static PI_Controller pi_diff_a = {0.01f, 0.25f, 0.0f, 0.0f, 6.0f};

/* ===== PI incrementale ===== */
static float PI_Compute(PI_Controller *pi, float error, float dt)
{
    float u = pi->u_prev + (pi->Kp * (error - pi->e_prev)) + (pi->Ki * dt * error);

    if (u >  pi->out_limit){
    	u =  pi->out_limit;
    }

    if (u < -pi->out_limit){
    	u = -pi->out_limit;
    }

    pi->e_prev = error;
    pi->u_prev = u;

    return u;
}

void ControlLaw_Init(void)
{
    pi_global.e_prev    = 0.0f;
    pi_global.u_prev    = 0.0f;

    pi_sync_asse.e_prev = 0.0f;
    pi_sync_asse.u_prev = 0.0f;

    pi_diff_p.e_prev    = 0.0f;
    pi_diff_p.u_prev    = 0.0f;

    pi_diff_a.e_prev    = 0.0f;
    pi_diff_a.u_prev    = 0.0f;
}


void ControlLaw_Step(const ControlInput_t *in, ControlOutput_t *out){
    float rpm_avg_p;
    float rpm_avg_a;
    float rpm_global;
    float error_diff_p;
    float error_diff_a;
    float u_base;
    float correction_asse;
    float u_target_p;
    float u_target_a;
    float diff_p;
    float diff_a;

    rpm_avg_p  = (in->rpm_dx_p + in->rpm_sx_p) * 0.5f;
	rpm_avg_a  = (in->rpm_dx_a + in->rpm_sx_a) * 0.5f;
	rpm_global = (rpm_avg_p + rpm_avg_a) * 0.5f;

	error_diff_p = (in->rpm_dx_p - in->rpm_sx_p) - (2.0f * in->steering_cmd);

	error_diff_a = (in->rpm_dx_a - in->rpm_sx_a) - (2.0f * in->steering_cmd);

	u_base = PI_Compute(&pi_global, (in->speed_ref_rpm - rpm_global), Ts);

	correction_asse = PI_Compute(&pi_sync_asse, (rpm_avg_a - rpm_avg_p), Ts);

	u_target_p = u_base + correction_asse;
	u_target_a = u_base - correction_asse;

	diff_p = PI_Compute(&pi_diff_p, error_diff_p, Ts);
	diff_a = PI_Compute(&pi_diff_a, error_diff_a, Ts);

	out->u_dx_p = u_target_p - diff_p;
	out->u_sx_p = u_target_p + diff_p;
	out->u_dx_a = u_target_a - diff_a;
	out->u_sx_a = u_target_a + diff_a;
}





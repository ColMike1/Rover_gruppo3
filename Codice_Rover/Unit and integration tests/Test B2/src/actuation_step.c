/**
 * @file actuation_step.c
 * @brief Step di attuazione open-loop per board 2.
 */

#include "actuation.h"

/** @brief Velocita' nominale massima usata per scalare i riferimenti [rpm]. */
#define MAX_SPEED_RPM 160.0f
/** @brief Limite di velocita' effettivo applicato in questa board [rpm]. */
#define MAX_SPEED_DEGRADED_RPM 80.0f
/** @brief Variazione massima ammessa a ogni step [rpm/step]. */
#define MAX_DELTA_RPM 4.0f
/** @brief Tensione massima di riferimento inviata al driver motori [V]. */
#define MAX_VOLTAGE 12.0f

float u_left_rpm_last = 0.0f;
float u_right_rpm_last = 0.0f;

/**
 * @brief Limita la variazione tra due comandi consecutivi.
 * @param target Nuovo valore desiderato.
 * @param prev Ultimo valore applicato.
 * @param max_delta Incremento/decremento massimo consentito per step.
 * @return Valore limitato da applicare in questo step.
 */
static float rate_limit(float target, float prev, float max_delta)
{
    float delta = target - prev;

    if (delta > max_delta)
    {
        delta = max_delta;
    }
    else if (delta < -max_delta)
    {
        delta = -max_delta;
    }

    return prev + delta;
}

/**
 * @brief Inizializza il modulo di attuazione.
 */
void Actuation_Init(void)
{
    Sabertooth_Init();
}

/**
 * @brief Esegue uno step di attuazione open-loop per la board 2.
 * @param v_ref Riferimento di velocita' lineare normalizzato (in [-1, 1]).
 * @param omega_ref Riferimento di velocita' angolare normalizzato (in [-1, 1]).
 */
void Actuation_Step(float v_ref, float omega_ref)
{
    /* Ultimi comandi applicati alle ruote [rpm]. */


    float v_rpm = v_ref * MAX_SPEED_RPM;
    float omega_rpm = omega_ref * MAX_SPEED_RPM;

    float u_left_rpm = v_rpm - omega_rpm;

    /* Saturazione della velocità a quella massima consentita. */
    if (u_left_rpm > MAX_SPEED_DEGRADED_RPM)
    {
        u_left_rpm = MAX_SPEED_DEGRADED_RPM;
    }
    if (u_left_rpm < -MAX_SPEED_DEGRADED_RPM)
    {
        u_left_rpm = -MAX_SPEED_DEGRADED_RPM;
    }

    float u_right_rpm = v_rpm + omega_rpm;
    if (u_right_rpm > MAX_SPEED_DEGRADED_RPM)
    {
        u_right_rpm = MAX_SPEED_DEGRADED_RPM;
    }
    if (u_right_rpm < -MAX_SPEED_DEGRADED_RPM)
    {
        u_right_rpm = -MAX_SPEED_DEGRADED_RPM;
    }

    /* Rate limiting per rendere piu' graduale accelerazione/decelerazione. */
    u_left_rpm = rate_limit(u_left_rpm, u_left_rpm_last, MAX_DELTA_RPM);
    u_right_rpm = rate_limit(u_right_rpm, u_right_rpm_last, MAX_DELTA_RPM);

    u_left_rpm_last = u_left_rpm;
    u_right_rpm_last = u_right_rpm;

    /* Conversione rpm -> tensione [V]. */
    float u_left_v = MAX_VOLTAGE * u_left_rpm / MAX_SPEED_RPM;
    float u_right_v = MAX_VOLTAGE * u_right_rpm / MAX_SPEED_RPM;

    /* Invio comandi al driver motori (stessa interfaccia usata su board 1). */
    Sabertooth_ApplyOutputs(u_left_v, u_right_v, u_left_v, u_right_v);
}

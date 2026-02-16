/**
 * @file control_step.c
 * @brief Orchestrazione del task di controllo e gestione delle strategie di fallback.
 * @details Si occupa di processare gli snapshot di sistema, decidere tra controllo
 * ad anello chiuso o aperto, e gestire i guasti agli encoder.
 */

#include <actuation/actuation_step.h>
#include "control/control.h"
#include "snapshot/encoder_snapshot.h"
#include "snapshot/supervisor_snapshot.h"
#include "snapshot/rx_snapshot.h"
#include <math.h>
#include "cmsis_os2.h"
#include <stdbool.h>

/** @brief Velocità massima nominale dei motori [RPM]. */
#define MAX_SPEED_RPM  160.0f
/** @brief Massimo differenziale di sterzata ammesso [RPM]. */
#define MAX_TURN_RPM   80.0f
/** @brief Limite cautelativo di velocità in modalità Open Loop [RPM]. */
#define OPEN_LOOP_LIMIT 80.0f
/** @brief Tensione massima di alimentazione motori [V]. */
#define MAX_ACTUATION_VOLTAGE 12.0f

/**
 * @brief Applica strategie di compensazione in caso di perdita di feedback encoder.
 * @details In base al numero e alla posizione dei sensori guasti, copia i comandi
 * di tensione dalle ruote funzionanti a quelle in avaria per mantenere la trazione.
 * @param[in,out] cmd Puntatore ai comandi di attuazione da modificare.
 * @param[in] sup Snapshot del supervisore contenente i flag di errore.
 */
static void Apply_Encoders_Fallback_Actuation(ControlOutput_t * const cmd, const SupervisorSnapshot_t sup)
{
    /* Rule 14.4: L'espressione dell'if deve essere di tipo booleano */
    const bool fault_fl = ((sup.degraded_mask & FAULT_WHEEL_FL) != 0U) || ((sup.critical_mask & FAULT_WHEEL_FL) != 0U);
    const bool fault_fr = ((sup.degraded_mask & FAULT_WHEEL_FR) != 0U) || ((sup.critical_mask & FAULT_WHEEL_FR) != 0U);
    const bool fault_rl = ((sup.degraded_mask & FAULT_WHEEL_RL) != 0U) || ((sup.critical_mask & FAULT_WHEEL_RL) != 0U);
    const bool fault_rr = ((sup.degraded_mask & FAULT_WHEEL_RR) != 0U) || ((sup.critical_mask & FAULT_WHEEL_RR) != 0U);

    unsigned int n_fail = 0U;
    if (fault_fl) { n_fail++; }
    if (fault_fr) { n_fail++; }
    if (fault_rl) { n_fail++; }
    if (fault_rr) { n_fail++; }

    /* === 4 FAIL === */
    if (n_fail == 4U)
    {
        /* Fallimento critico: nessuna azione possibile qui */
    }
    /* === 3 FAIL === */
    else if (n_fail == 3U)
    {
        float ref;
        if (!fault_fl)      { ref = cmd->u_sx_a; }
        else if (!fault_fr) { ref = cmd->u_dx_a; }
        else if (!fault_rl) { ref = cmd->u_sx_p; }
        else                { ref = cmd->u_dx_p; }

        cmd->u_sx_a = ref;
        cmd->u_dx_a = ref;
        cmd->u_sx_p = ref;
        cmd->u_dx_p = ref;
    }
    /* === 2 FAIL === */
    else if (n_fail == 2U)
    {
        /* Stesso lato */
        if (fault_fl && fault_rl)
        {
            cmd->u_sx_a = cmd->u_dx_a;
            cmd->u_sx_p = cmd->u_dx_p;
        }
        else if (fault_fr && fault_rr)
        {
            cmd->u_dx_a = cmd->u_sx_a;
            cmd->u_dx_p = cmd->u_sx_p;
        }
        /* Stesso asse */
        else if (fault_rl && fault_rr)
        {
            cmd->u_sx_p = cmd->u_sx_a;
            cmd->u_dx_p = cmd->u_dx_a;
        }
        else if (fault_fl && fault_fr)
        {
            cmd->u_sx_a = cmd->u_sx_p;
            cmd->u_dx_a = cmd->u_dx_p;
        }
        /* Incrociati */
        else if (fault_fl && fault_rr)
        {
            cmd->u_sx_a = cmd->u_sx_p;
            cmd->u_dx_p = cmd->u_dx_a;
        }
        else if (fault_fr && fault_rl)
        {
            cmd->u_dx_a = cmd->u_dx_p;
            cmd->u_sx_p = cmd->u_sx_a;
        }
        else
        {
            /* Ramo else finale per motivi di sicurezza */
        }
    }
    /* === 1 FAIL === */
    else if (n_fail == 1U)
    {
        if (fault_fl)      { cmd->u_sx_a = cmd->u_sx_p; }
        else if (fault_fr) { cmd->u_dx_a = cmd->u_dx_p; }
        else if (fault_rl) { cmd->u_sx_p = cmd->u_sx_a; }
        else if (fault_rr) { cmd->u_dx_p = cmd->u_dx_a; }
        else { /* Default */ }
    }
    else
    {
        /* Nessun errore */
    }
}

/**
 * @brief Inizializzazione del modulo di controllo.
 */
void Control_Init(void)
{
    /* Inizializza stati interni della control law */
    ControlLaw_Init();
}

/**
 * @brief Esegue lo step periodico di controllo.
 * @details
 * 1. Legge gli snapshot (Encoder, Supervisor, RX).
 * 2. Normalizza i setpoint di velocità e sterzata.
 * 3. Se tutti gli encoder sono guasti, entra in modalità **Open Loop** (Anello aperto).
 * 4. Altrimenti, esegue la **Control Law** e applica eventuali fallback per ruote singole.
 * @return ControlOutput_t Struttura contenente i comandi di tensione per i motori.
 */
ControlOutput_t Control_Step(void)
{
    static EncoderSnapshot_t enc;
    static SupervisorSnapshot_t sup;
    static RxSnapshot_t rx;

    /* === Lettura snapshot === */
    EncoderSnapshot_Read(&enc);
    SupervisorSnapshot_Read(&sup);
    RxSnapshot_Read(&rx);

    ControlInput_t  in;
    ControlOutput_t out = {0.0f, 0.0f, 0.0f, 0.0f};

    in.speed_ref_rpm = sup.speed_ref_rpm * MAX_SPEED_RPM;
    in.steering_cmd  = sup.steering_cmd * MAX_TURN_RPM;

    const float total_max_requested = fabsf(in.speed_ref_rpm) + fabsf(in.steering_cmd);

    if (total_max_requested > MAX_SPEED_RPM)
    {
        const float scale = MAX_SPEED_RPM / total_max_requested;
        in.speed_ref_rpm *= scale;
        in.steering_cmd *= scale;
    }

    in.rpm_sx_a = enc.wheel_speed_rpm[0];
    in.rpm_dx_a = enc.wheel_speed_rpm[1];
    in.rpm_sx_p = enc.wheel_speed_rpm[2];
    in.rpm_dx_p = enc.wheel_speed_rpm[3];

    const uint32_t all_fault_mask = FAULT_WHEEL_FL | FAULT_WHEEL_FR | FAULT_WHEEL_RL | FAULT_WHEEL_RR;

    const bool use_open_loop = (sup.degraded_mask & all_fault_mask) == all_fault_mask;

    const bool disable_steering = ((sup.degraded_mask & (FAULT_WHEEL_FL | FAULT_WHEEL_FR)) == (FAULT_WHEEL_FL | FAULT_WHEEL_FR)) ||
                                  ((sup.degraded_mask & (FAULT_WHEEL_RL | FAULT_WHEEL_RR)) == (FAULT_WHEEL_RL | FAULT_WHEEL_RR));

    if (disable_steering && (!use_open_loop))
    {
        in.steering_cmd = 0.0f;
    }

    if (use_open_loop)
    {
        float u_left_rpm  = in.speed_ref_rpm - in.steering_cmd;
        float u_right_rpm = in.speed_ref_rpm + in.steering_cmd;

        /* Rule 17.7: Valore di ritorno di funzioni non ignorato (se applicabile) */
        if (u_left_rpm > OPEN_LOOP_LIMIT)  
        {
             u_left_rpm = OPEN_LOOP_LIMIT; 
        }

        if (u_left_rpm < -OPEN_LOOP_LIMIT) 
        {
            u_left_rpm = -OPEN_LOOP_LIMIT; 
        }

        if (u_right_rpm > OPEN_LOOP_LIMIT)  
        { 
            u_right_rpm = OPEN_LOOP_LIMIT; 
        }

        if (u_right_rpm < -OPEN_LOOP_LIMIT) 
        { 
            u_right_rpm = -OPEN_LOOP_LIMIT; 
        }

        /* Scaling rpm -> voltaggio */
        float u_left_v = (MAX_ACTUATION_VOLTAGE * u_left_rpm) / MAX_SPEED_RPM;
        float u_right_v = (MAX_ACTUATION_VOLTAGE * u_right_rpm) / MAX_SPEED_RPM;

        out.u_sx_a = u_left_v;
        out.u_dx_a = u_right_v;
        out.u_sx_p = u_left_v;
        out.u_dx_p = u_right_v;
    }
    else
    {
        ControlLaw_Step(&in, &out);
    }

    /* Gestione Fallback Encoders */
    const bool any_wheel_fault = ((sup.degraded_mask & all_fault_mask) != 0U) ||
                                 ((sup.critical_mask & all_fault_mask) != 0U);

    if (any_wheel_fault && (!use_open_loop))
    {
        Apply_Encoders_Fallback_Actuation(&out, sup);
    }

    return out;
}

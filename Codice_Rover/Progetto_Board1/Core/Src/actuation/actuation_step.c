/**
 * @file actuation_step.c
 * @brief Implementazione del task di attuazione per il controllo motori.
 */

#include "actuation/actuation.h"
#include "snapshot/supervisor_snapshot.h"
#include "snapshot/encoder_snapshot.h"
#include <control/control_law.h>
#include <math.h>

/* === Parametri === */
/** @brief Tensione massima applicabile ai motori [V]. */
#define ACTUATION_MAX_VOLTAGE   12.0f
/** @brief Soglia di zona morta per la tensione [V]. */
#define VOLTAGE_DEADZONE        0.6f
/** @brief Indice feedback ruota anteriore sinistra. */
#define FEEDBACK_IDX_SX_A       0U
/** @brief Indice feedback ruota anteriore destra. */
#define FEEDBACK_IDX_DX_A       1U
/** @brief Indice feedback ruota posteriore sinistra. */
#define FEEDBACK_IDX_SX_P       2U
/** @brief Indice feedback ruota posteriore destra. */
#define FEEDBACK_IDX_DX_P       3U

/**
 * @brief Inizializza l'hardware di attuazione.
 * Richiama la configurazione periferica del driver Sabertooth.
 */
void Actuation_Init(void)
{
    Sabertooth_Init();
}

/**
 * @brief Esegue lo step periodico di attuazione.
 * Legge gli snapshot di sistema, applica filtri di zona morta (deadzone)
 * in assenza di feedback e satura i segnali di comando entro i limiti definiti
 * prima di inviarli ai driver motori.
 * @param cmd Struttura contenente i comandi di tensione calcolati dalla legge di controllo.
 */
void Actuation_Step(ControlOutput_t cmd)
{
    static SupervisorSnapshot_t sup;
    static EncoderSnapshot_t enc;

    float u_sx_a;
    float u_dx_a;
    float u_sx_p;
    float u_dx_p;

    /* Lettura degli snapshot. */
    SupervisorSnapshot_Read(&sup);
    EncoderSnapshot_Read(&enc);

    u_sx_a = cmd.u_sx_a;
    u_dx_a = cmd.u_dx_a;
    u_sx_p = cmd.u_sx_p;
    u_dx_p = cmd.u_dx_p;

    /* Applicazione deadzone se manca feedback. */
    if ((fabsf(u_sx_a) < VOLTAGE_DEADZONE) && (enc.hasNoFeedback[FEEDBACK_IDX_SX_A] != 0U))
    {
        u_sx_a = 0.0f;
    }

    if ((fabsf(u_dx_a) < VOLTAGE_DEADZONE) && (enc.hasNoFeedback[FEEDBACK_IDX_DX_A] != 0U))
    {
        u_dx_a = 0.0f;
    }

    if ((fabsf(u_sx_p) < VOLTAGE_DEADZONE) && (enc.hasNoFeedback[FEEDBACK_IDX_SX_P] != 0U))
    {
        u_sx_p = 0.0f;
    }

    if ((fabsf(u_dx_p) < VOLTAGE_DEADZONE) && (enc.hasNoFeedback[FEEDBACK_IDX_DX_P] != 0U))
    {
        u_dx_p = 0.0f;
    }

    /* Saturazione dell'uscita. */
    if (u_sx_a > ACTUATION_MAX_VOLTAGE)
    {
        u_sx_a = ACTUATION_MAX_VOLTAGE;
    }
    if (u_sx_a < -ACTUATION_MAX_VOLTAGE)
    {
        u_sx_a = -ACTUATION_MAX_VOLTAGE;
    }

    if (u_dx_a > ACTUATION_MAX_VOLTAGE)
    {
        u_dx_a = ACTUATION_MAX_VOLTAGE;
    }
    if (u_dx_a < -ACTUATION_MAX_VOLTAGE)
    {
        u_dx_a = -ACTUATION_MAX_VOLTAGE;
    }

    if (u_sx_p > ACTUATION_MAX_VOLTAGE)
    {
        u_sx_p = ACTUATION_MAX_VOLTAGE;
    }
    if (u_sx_p < -ACTUATION_MAX_VOLTAGE)
    {
        u_sx_p = -ACTUATION_MAX_VOLTAGE;
    }

    if (u_dx_p > ACTUATION_MAX_VOLTAGE)
    {
        u_dx_p = ACTUATION_MAX_VOLTAGE;
    }
    if (u_dx_p < -ACTUATION_MAX_VOLTAGE)
    {
        u_dx_p = -ACTUATION_MAX_VOLTAGE;
    }

    /*Attuazione HW. */
    /** @note Invia i segnali saturati e corretti al driver Sabertooth. */
    Sabertooth_ApplyOutputs(u_sx_a, u_dx_a, u_sx_p, u_dx_p);
}

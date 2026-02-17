/**
 * @file encoder_task.c
 * @brief Task di elaborazione dati encoder e diagnostica feedback motori.
 * @details Converte i ticks in RPM, rileva situazioni di stallo (mancanza di feedback)
 * e applica logiche di fallback se alcuni sensori sono dichiarati guasti dal supervisore.
 * @date Jan 8, 2026
 * @author Sterm
 */

#include "encoder.h"
#include "encoder_snapshot.h"
#include "supervisor_snapshot.h"
#include "cmsis_os2.h"
#include <stdbool.h>
#include "tim.h"

#define ENCODER_TASK_PERIOD_S    0.01f
#define NUM_ENCODERS             4U
#define TICKS_PER_REV            2448.0f

/** @brief Tolleranza per considerare la ruota ferma [RPM]. */
#define RPM_EPS        2.5f
/** @brief Tensione minima di attivazione per il controllo di stallo [V]. */
#define U_TRIG         2.0f
/** @brief Numero di cicli consecutivi di errore per dichiarare il guasto feedback. */
#define N_FAIL         20U

/**
 * @brief Converte i ticks misurati in velocità angolare (RPM).
 * @param delta_ticks Variazione dei ticks nel periodo.
 * @return float Velocità calcolata in RPM.
 */


static float DeltaTicks_ToRPM(int delta_ticks)
{
  return ((float)delta_ticks * 60.0f) / (TICKS_PER_REV * ENCODER_TASK_PERIOD_S);
}

/**
 * @brief Calcola il valore assoluto di un float.
 * @param x Valore in ingresso.
 * @return float Valore assoluto.
 */
static float abs_f32(float x)
{
    return (x < 0.0f) ? -x : x;
}

/**
 * @brief Avvia l'interfaccia encoder dei timer hardware.
 */
void Encoder_Init(void)
{
    /* Rule 17.7: Valore di ritorno dell'HAL non ignorato implicitamente */
    (void)HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    (void)HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    (void)HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    (void)HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

/**
 * @brief Diagnostica la presenza di feedback dall'encoder.
 * @details Implementa un contatore di stallo: se la tensione di comando è alta
 * ma i giri letti sono quasi nulli per N_FAIL cicli, viene segnalato un guasto.
 * @param idx Indice dell'encoder.
 * @param u_cmd Comando di tensione inviato nell'ultimo ciclo.
 * @param rpm Velocità misurata.
 * @return true Se è stato rilevato un guasto o uno stallo persistente.
 */
static bool Encoder_hasNoFeedback(uint8_t idx, float u_cmd, float rpm)
{
	static uint8_t stall_cnt[NUM_ENCODERS] = { 0U, 0U, 0U, 0U };
    bool fault = false;

    if ((stall_cnt[idx] >= N_FAIL) && (abs_f32(u_cmd) < U_TRIG))
    {
        fault = true;
    }
    else
    {
        if ((abs_f32(u_cmd) > U_TRIG) && (abs_f32(rpm) < RPM_EPS))
        {
            if (stall_cnt[idx] < N_FAIL)
            {
                stall_cnt[idx]++;
            }
        }
        else
        {
            stall_cnt[idx] = 0U;
        }

        if (stall_cnt[idx] >= N_FAIL)
        {
            fault = true;
        }
    }

    return fault;
}

/**
 * @brief Logica di fallback per la stima dei giri ruota in caso di guasto ai sensori.
 * @details Se uno o più encoder falliscono, i giri mancanti vengono stimati basandosi
 * sulle letture delle ruote sane (stesso lato, stesso asse o incrociate).
 * @param[in,out] snap Snapshot degli encoder da correggere.
 * @param[in] sup Snapshot del supervisore per conoscere lo stato dei guasti.
 */
static void Apply_Encoders_Fallback(EncoderSnapshot_t * const snap, const SupervisorSnapshot_t sup)
{
    /* Rule 14.4: Confronto esplicito per espressione booleana */
    const bool fault_fl = ((sup.degraded_mask & FAULT_WHEEL_FL) != 0U) || ((sup.critical_mask & FAULT_WHEEL_FL) != 0U);
    const bool fault_fr = ((sup.degraded_mask & FAULT_WHEEL_FR) != 0U) || ((sup.critical_mask & FAULT_WHEEL_FR) != 0U);
    const bool fault_rl = ((sup.degraded_mask & FAULT_WHEEL_RL) != 0U) || ((sup.critical_mask & FAULT_WHEEL_RL) != 0U);
    const bool fault_rr = ((sup.degraded_mask & FAULT_WHEEL_RR) != 0U) || ((sup.critical_mask & FAULT_WHEEL_RR) != 0U);

    const uint8_t n_fail = (uint8_t)((fault_fl ? 1U : 0U) + (fault_fr ? 1U : 0U) + (fault_rl ? 1U : 0U) + (fault_rr ? 1U : 0U));

    float * const rpm_fl = &snap->wheel_speed_rpm[0];
    float * const rpm_fr = &snap->wheel_speed_rpm[1];
    float * const rpm_rl = &snap->wheel_speed_rpm[2];
    float * const rpm_rr = &snap->wheel_speed_rpm[3];

    /* === 4 FAIL === */
    if (n_fail == 4U)
    {
        /* Fallimento critico, non ho riferimenti per il pid, unica opzione usare open loop. */
    }
    /* === 3 FAIL === */
    else if (n_fail == 3U)
    {
        float ref;
        if (!fault_fl)      { ref = *rpm_fl; }
        else if (!fault_fr) { ref = *rpm_fr; }
        else if (!fault_rl) { ref = *rpm_rl; }
        else                { ref = *rpm_rr; }

        *rpm_fl = ref;
        *rpm_fr = ref;
        *rpm_rl = ref;
        *rpm_rr = ref;
    }
    /* === 2 FAIL === */
    else if (n_fail == 2U)
    {
        /* stesso lato */
        if (fault_fl && fault_rl)
        {
            *rpm_fl = *rpm_fr;
            *rpm_rl = *rpm_rr;
        }
        else if (fault_fr && fault_rr)
        {
            *rpm_fr = *rpm_fl;
            *rpm_rr = *rpm_rl;
        }
        /* stesso asse */
        else if (fault_rl && fault_rr)
        {
            *rpm_rl = *rpm_fl;
            *rpm_rr = *rpm_fr;
        }
        else if (fault_fl && fault_fr)
        {
            *rpm_fl = *rpm_rl;
            *rpm_fr = *rpm_rr;
        }
        /* incrociati */
        else if (fault_fl && fault_rr)
        {
            *rpm_fl = *rpm_rl;
            *rpm_rr = *rpm_fr;
        }
        else if(fault_fr && fault_rl)
        {
            *rpm_fr = *rpm_rr;
            *rpm_rl = *rpm_fl;
        }
        else
        {
            /* Case protettivo obbligatorio */
        }
    }
    else if (n_fail == 1U)
    {
        if (fault_fl)      
        { 
            *rpm_fl = *rpm_rl; 
        }
        else if (fault_fr) 
        { 
            *rpm_fr = *rpm_rr; 
        }
        else if (fault_rl) 
        {
             *rpm_rl = *rpm_fl; 
        }
        else if (fault_rr) 
        {
            *rpm_rr = *rpm_fr;
        }
        else 
        { /* No action */ 
        }
    }
    else
    { /* Nessun fallimento */
    }
}

/**
 * @brief Step periodico del task encoder.
 * @details
 * 1. Legge i delta ticks hardware per i 4 motori.
 * 2. Converte i dati in RPM e aggiorna i timestamp.
 * 3. Esegue la diagnostica dello stallo basata sul comando del ciclo precedente.
 * 4. Se necessario, applica le correzioni di fallback.
 * 5. Scrive lo snapshot finale per gli altri task.
 * @param last_cycle_cmd Array dei comandi di tensione [V] applicati nell'ultima iterazione.
 */
void Encoder_Step(float last_cycle_cmd[4])
{
  static EncoderSnapshot_t snap;
  static SupervisorSnapshot_t sup;
  uint32_t now = osKernelGetTickCount();
  uint8_t i;

  snap.task_last_run_ms = now;

  for (i = 0U; i < NUM_ENCODERS; i++)
  {
    int delta = 0;
    EncoderStatus_t st = Encoder_ReadDelta(i, &delta);

    if (st == ENCODER_OK)
    {
      snap.wheel_speed_rpm[i] = DeltaTicks_ToRPM(delta);
      snap.data_last_valid_ms[i] = now;
      snap.hasNoFeedback[i] = Encoder_hasNoFeedback(i, last_cycle_cmd[i], snap.wheel_speed_rpm[i]);
    }
    else
    {
        snap.hasNoFeedback[i] = true;
    }
  }

  SupervisorSnapshot_Read(&sup);

  if (((sup.degraded_mask & 0x0000000FU) != 0U) || ((sup.critical_mask & 0x0000000FU) != 0U))
  {
      Apply_Encoders_Fallback(&snap, sup);
  }

  EncoderSnapshot_Write(&snap);
}

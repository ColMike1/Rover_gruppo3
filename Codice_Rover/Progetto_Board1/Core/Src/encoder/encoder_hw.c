/**
 * @file encoder_hw.c
 * @brief Interfaccia di basso livello per la lettura degli encoder hardware via timer.
 * @details Gestisce il calcolo del delta dei ticks, risolvendo le problematiche di
 * overflow dei timer a 16 bit e verificando la plausibilità fisica delle letture.
 * @date Jan 8, 2026
 * @author Sterm
 */

#include "encoder/encoder_hw.h"
#include "tim.h"

/* === CONFIG === */
/** @brief Valore massimo del timer encoder (16-bit). */
#define ENCODER_TIMER_MAX     65536.0f
/** @brief Velocità massima attesa per ruota [RPM] per controlli di plausibilità. */
#define MAX_RPM               200.0f
/** @brief Risoluzione dell'encoder (ticks per rivoluzione completa). */
#define TICKS_PER_REV         2448.0f
/** @brief Periodo di campionamento del task encoder [s]. */
#define ENCODER_TASK_PERIOD_S   0.010f

/** @brief Massimo numero di ticks fisicamente possibili in un periodo di campionamento. */
#define MAX_DELTA_TICKS ((int)((MAX_RPM * TICKS_PER_REV / 60.0f) * ENCODER_TASK_PERIOD_S) + 1)

/** @brief Numero totale di encoder gestiti dal sistema. */
#define NUM_ENCODERS 4U

/* 4 encoders on 4 timers */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/** @brief Array di puntatori ai timer hardware configurati in Encoder Mode. */
static TIM_HandleTypeDef *enc_timers[NUM_ENCODERS] =
{
  &htim2, &htim4, &htim1, &htim3
};

/** @brief Buffer interno per memorizzare l'ultima lettura dei ticks per ogni encoder. */
static uint32_t last_ticks[NUM_ENCODERS] = {0U, 0U, 0U, 0U};

/**
 * @brief Legge la variazione (delta) dei ticks dall'ultima chiamata per un dato encoder.
 * @details Calcola la differenza gestendo l'overflow del timer circolare e verifica
 * che il movimento non superi i limiti fisici definiti da MAX_DELTA_TICKS.
 * @param[in] encoder_id Indice dell'encoder (0 a 3).
 * @param[out] delta_ticks Puntatore alla variabile dove scrivere il delta calcolato.
 * @return EncoderStatus_t OK se la lettura è valida, OVERFLOW se supera i limiti fisici.
 */
EncoderStatus_t Encoder_ReadDelta(uint8_t encoder_id, int *delta_ticks)
{
  EncoderStatus_t status = ENCODER_OK;
  TIM_HandleTypeDef *htim = enc_timers[encoder_id];

  uint32_t current = __HAL_TIM_GET_COUNTER(htim);
  uint32_t last    = last_ticks[encoder_id];

  int delta_u;

  /* -------- Overflow / underflow (numerico) -------- */
  /* Rule 10.1: Conversione esplicita per evitare operazioni tra signed/unsigned */
  delta_u = (int)current - (int)last;

  if ((float)delta_u > (ENCODER_TIMER_MAX / 2.0f))
  {
      delta_u -= (int)ENCODER_TIMER_MAX;
  }
  else if ((float)delta_u < -(ENCODER_TIMER_MAX / 2.0f))
  {
      delta_u += (int)ENCODER_TIMER_MAX;
  }
  else
  {
      /* Valore nel range corretto */
  }

  last_ticks[encoder_id] = current;

  /* -------- Plausibility fisica -------- */
  if ((delta_u > MAX_DELTA_TICKS) || (delta_u < -MAX_DELTA_TICKS))
  {
      status = ENCODER_OVERFLOW;
  }
  else
  {
      if (delta_ticks != (void *)0)
      {
          *delta_ticks = delta_u;
      }
      status = ENCODER_OK;
  }

  return status;
}

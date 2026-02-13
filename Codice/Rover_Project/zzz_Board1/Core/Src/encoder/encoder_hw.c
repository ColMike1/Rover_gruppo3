/*
 * encoder_hw.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */


#include "encoder/encoder_hw.h"
#include "tim.h"

/* === CONFIG === */
#define ENCODER_TIMER_MAX     65536.0f    /* 16-bit encoder timer */
#define MAX_RPM               200.0f     /* worst-case expected */
#define TICKS_PER_REV         2448.0f
#define ENCODER_TASK_PERIOD_S   0.010f

/* max ticks physically possible in one period */
#define MAX_DELTA_TICKS ((int32_t)((MAX_RPM * TICKS_PER_REV / 60.0f) * ENCODER_TASK_PERIOD_S) + 1)

#define NUM_ENCODERS 4U
/* 4 encoders on 4 timers */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

static TIM_HandleTypeDef *enc_timers[NUM_ENCODERS] =
{
  &htim2, &htim4, &htim1, &htim3
};

static uint32_t last_ticks[NUM_ENCODERS] = {0U, 0U, 0U, 0U};

EncoderStatus_t Encoder_ReadDelta(uint8_t encoder_id, int32_t *delta_ticks)
{

  TIM_HandleTypeDef *htim = enc_timers[encoder_id];

  uint32_t current = __HAL_TIM_GET_COUNTER(htim);
  uint32_t last    = last_ticks[encoder_id];

  int32_t delta_u;

  /* -------- Overflow / underflow (numerico) -------- */
  delta_u = current - last;
  if (delta_u > (ENCODER_TIMER_MAX / 2.0f))
      delta_u -= ENCODER_TIMER_MAX;
  else if (delta_u < -(ENCODER_TIMER_MAX/ 2.0f))
      delta_u += ENCODER_TIMER_MAX;

  last_ticks[encoder_id] = current;

  /* -------- Plausibility fisica -------- */
  if (delta_u > MAX_DELTA_TICKS || delta_u < -MAX_DELTA_TICKS)
    return ENCODER_OVERFLOW;

  *delta_ticks = delta_u;

  return ENCODER_OK;
}


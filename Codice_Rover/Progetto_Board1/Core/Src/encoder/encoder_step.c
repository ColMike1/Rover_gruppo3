/*
 * encoder_task.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */


#include "encoder/encoder.h"
#include "snapshot/encoder_snapshot.h"
#include "snapshot/supervisor_snapshot.h"

#include "cmsis_os2.h"
#include <stdbool.h>
#include "tim.h"


#define ENCODER_TASK_PERIOD_S    0.01f
#define NUM_ENCODERS             4U
#define TICKS_PER_REV            2448.0f

#define RPM_EPS        2.5f      /* 1 tick/10ms ~ 2.45rpm */
#define U_TRIG         2.0f     /* comando "non piccolo" (2volt) */
#define N_FAIL         20U


static float DeltaTicks_ToRPM(int32_t delta_ticks)
{
  return ((float)delta_ticks * 60.0f) / (TICKS_PER_REV * ENCODER_TASK_PERIOD_S);
}

static float abs_f32(float x)
{
    return (x < 0.0f) ? -x : x;
}


/* ===== API ===== */
void Encoder_Init(void)
{
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

static bool Encoder_hasNoFeedback(uint8_t idx, float u_cmd, float rpm){
	static uint8_t stall_cnt[NUM_ENCODERS] = { 0U };
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

/* ===================== Fallback logic ===================== */
static void Apply_Encoders_Fallback(EncoderSnapshot_t *snap, SupervisorSnapshot_t sup){

    bool fault_fl = (sup.degraded_mask & FAULT_WHEEL_FL) || (sup.critical_mask & FAULT_WHEEL_FL) ;
    bool fault_fr = (sup.degraded_mask & FAULT_WHEEL_FR) || (sup.critical_mask & FAULT_WHEEL_FR);
    bool fault_rl = (sup.degraded_mask & FAULT_WHEEL_RL) || (sup.critical_mask & FAULT_WHEEL_RL);
    bool fault_rr = (sup.degraded_mask & FAULT_WHEEL_RR) || (sup.critical_mask & FAULT_WHEEL_RR);

    uint8_t n_fail =
            (uint8_t)((fault_fl ? 1U : 0U) + (fault_fr ? 1U : 0U) + (fault_rl ? 1U : 0U) + (fault_rr ? 1U : 0U));

    float *rpm_fl = &snap->wheel_speed_rpm[0];
    float *rpm_fr = &snap->wheel_speed_rpm[1];
    float *rpm_rl = &snap->wheel_speed_rpm[2];
    float *rpm_rr = &snap->wheel_speed_rpm[3];

    /* === 4 FAIL === */
    if (n_fail == 4U) {
    	/*
    	 * Fallimento critico, non ho riferimenti per il pid, unica opzione usare open loop.
    	 */
    	return;
    }

    /* === 3 FAIL === */
    if (n_fail == 3U) {
        float ref;
        if (!fault_fl){
        	ref = *rpm_fl;
        }
        else if (!fault_fr){
        	ref = *rpm_fr;
        }
        else if (!fault_rl){
        	ref = *rpm_rl;
        }
        else{
        	ref = *rpm_rr;
        }

        *rpm_fl = ref;
        *rpm_fr = ref;
        *rpm_rl = ref;
        *rpm_rr = ref;
        return;
    }

    /* === 2 FAIL === */
    if (n_fail == 2U) {

        /* stesso lato */
        if (fault_fl && fault_rl) {
            *rpm_fl = *rpm_fr;
            *rpm_rl = *rpm_rr;
            return ;
        }

        if (fault_fr && fault_rr) {
            *rpm_fr = *rpm_fl;
            *rpm_rr = *rpm_rl;
            return ;
        }


        /* stesso asse */
        if (fault_rl && fault_rr) {
            *rpm_rl = *rpm_fl;
            *rpm_rr = *rpm_fr;
            return ;
        }
        if (fault_fl && fault_fr) {
            *rpm_fl = *rpm_rl;
            *rpm_fr = *rpm_rr;
            return ;
        }

        /* incrociati */
        if (fault_fl && fault_rr){
        	*rpm_fl = *rpm_rl;
        	*rpm_rr = *rpm_fr;
        	return;
        }

        if(fault_fr && fault_rl){
        	*rpm_fr = *rpm_rr;
        	*rpm_rl = *rpm_fl;
        	return;
        }
    }

    if (n_fail == 1U){
		if (fault_fl){
			*rpm_fl = *rpm_rl;
			return;
		}
		if (fault_fr){
			*rpm_fr = *rpm_rr;
			return;
		}
		if (fault_rl){
			*rpm_rl = *rpm_fl;
			return;
		}
		if (fault_rr){
			*rpm_rr = *rpm_fr;
			return;
		}
    }
}


void Encoder_Step(float last_cycle_cmd[4])
{

  static EncoderSnapshot_t snap;
  uint32_t now = osKernelGetTickCount();
  snap.task_last_run_ms = now;

  uint8_t i;
  for (i = 0; i < NUM_ENCODERS; i++)
  {
    int32_t delta;

    EncoderStatus_t st = Encoder_ReadDelta(i, &delta);

    if (st == ENCODER_OK)
    {
      snap.wheel_speed_rpm[i] = DeltaTicks_ToRPM(delta);
      snap.data_last_valid_ms[i] = now;
      snap.hasNoFeedback[i] = Encoder_hasNoFeedback(i, last_cycle_cmd[i], snap.wheel_speed_rpm[i]);
    }
    else{
        snap.hasNoFeedback[i] = true;
    }

  }


  static SupervisorSnapshot_t sup;
  SupervisorSnapshot_Read(&sup);


  if( (sup.degraded_mask & (FAULT_WHEEL_FL | FAULT_WHEEL_FR | FAULT_WHEEL_RL | FAULT_WHEEL_RR))
		  || (sup.critical_mask & (FAULT_WHEEL_FL | FAULT_WHEEL_FR | FAULT_WHEEL_RL | FAULT_WHEEL_RR))){
	  Apply_Encoders_Fallback(&snap, sup);
  }

  EncoderSnapshot_Write(&snap);
}

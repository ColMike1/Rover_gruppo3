/*
 * sonar_task.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */



#include "sonar/sonar_task.h"
#include "sonar/sonar_hw.h"
#include "snapshot/sonar_snapshot.h"

#include "log/wcet_monitor.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

#define SONAR_TIMEOUT_MS  35
#define SONAR_MAX_CM      301

static TaskHandle_t sonarTaskHandle = NULL;

extern sonars_flag_t flag;

// Variabile per contare quanti sonar hanno completato la lettura
extern uint8_t sonar_count;

void Sonar_TaskInit(void)
{
    sonarTaskHandle = xTaskGetCurrentTaskHandle();
}

void Sonar_TaskStep(void)
{
    static SonarSnapshot_t snap;

    snap.task_last_run_ms = osKernelGetTickCount();

    scan();

    SonarHW_GetDistances(&snap.dist_cm[0], &snap.dist_cm[1], &snap.dist_cm[2]);

    SonarSnapshot_Write(&snap);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(htim->Instance == TIM1){
    	switch(htim->Channel){
    		case HAL_TIM_ACTIVE_CHANNEL_1:
    			if(flag.sonar1_ok == 0){
    				flag.sonar1_ok = 1;
    				sonar_count ++;
    			}
		        break;
    		case HAL_TIM_ACTIVE_CHANNEL_2:
    			if(flag.sonar2_ok == 0){
    				flag.sonar2_ok = 1;
					sonar_count ++;
    			}
		        break;
    		case HAL_TIM_ACTIVE_CHANNEL_3:
    			if(flag.sonar3_ok == 0){
    				flag.sonar3_ok = 1;
    				sonar_count ++;
    			}
		        break;
    		default:
    		        break;

    	}
    }

    if (sonar_count >= 3) {
        // Notifica il task e richiedi uno switch immediato se necessario
        xTaskNotifyFromISR(sonarTaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

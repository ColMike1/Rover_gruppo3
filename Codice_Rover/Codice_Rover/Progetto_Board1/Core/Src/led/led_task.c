/*
 * led_task.c
 *
 *  Created on: Jan 26, 2026
 *      Author: OEM
 */

#include "led/led_task.h"
#include "snapshot/supervisor_snapshot.h"
#include "cmsis_os2.h"
#include "string.h"
#include "gpio.h"
#include "snapshot/rx_snapshot.h"

static int n = 0U;

void Led_TaskInit(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
}

void Led_TaskStep(){
	static SupervisorSnapshot_t sup;
	static RxSnapshot_t rx;

	SupervisorSnapshot_Read(&sup);
	RxSnapshot_Read(&rx);

	if ((sup.critical_mask != 0U) || (rx.payload.critical_mask != 0U)){
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
	}
	else if (sup.degraded_mask != 0 || rx.payload.degraded_mask!=0){
		if(n == 5U){
			HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			n = 0U;
		}
		n ++;
	}
	else{
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
	}


	if(sup.current_action == CMD_GO_LEFT){
		HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_RESET);
	}


	if(sup.current_action == CMD_GO_RIGHT){
		HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_SET);

	}
	else{
		HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_RESET);
	}

	if(sup.current_action == CMD_ESTOP){
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
	}

}

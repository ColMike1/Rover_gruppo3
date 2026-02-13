/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "actuation/actuation_step.h"
#include "control/control_step.h"
#include "encoder/encoder_step.h"
#include "board_health/board_health_task.h"
#include "comm/comm_rx_task.h"
#include "comm/comm_tx_task.h"
#include "log/log_task.h"
#include "led/led_task.h"
#include "supervisor/supervisor_task.h"
#include "snapshot/snapshot.h"
#include "log/wcet_monitor.h"
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

UBaseType_t free_words_enc = 0;
UBaseType_t free_words_batt = 0;
UBaseType_t free_words_tx = 0;
UBaseType_t free_words_act = 0;
UBaseType_t free_words_ctrl = 0;

volatile uint32_t wcet_enc_cycles_max = 0;
volatile uint32_t wcet_batt_cycles_max = 0;
volatile uint32_t wcet_tx_cycles_max = 0;
volatile uint32_t wcet_act_cycles_max = 0;
volatile uint32_t wcet_ctrl_cycles_max = 0;

volatile uint32_t cycles_batt = 0;
volatile uint32_t cycles_enc = 0;
volatile uint32_t cycles_tx = 0;
volatile uint32_t cycles_act = 0;
volatile uint32_t cycles_ctrl = 0;


volatile uint32_t indice = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_BattTemp */
osThreadId_t Task_BattTempHandle;
uint32_t Task_BattTempBuffer[ 512 ];
osStaticThreadDef_t Task_BattTempControlBlock;
const osThreadAttr_t Task_BattTemp_attributes = {
  .name = "Task_BattTemp",
  .stack_mem = &Task_BattTempBuffer[0],
  .stack_size = sizeof(Task_BattTempBuffer),
  .cb_mem = &Task_BattTempControlBlock,
  .cb_size = sizeof(Task_BattTempControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_TX */
osThreadId_t Task_TXHandle;
uint32_t Task_TXBuffer[ 512 ];
osStaticThreadDef_t Task_TXControlBlock;
const osThreadAttr_t Task_TX_attributes = {
  .name = "Task_TX",
  .stack_mem = &Task_TXBuffer[0],
  .stack_size = sizeof(Task_TXBuffer),
  .cb_mem = &Task_TXControlBlock,
  .cb_size = sizeof(Task_TXControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_RX */
osThreadId_t Task_RXHandle;
uint32_t Task_RXBuffer[ 768 ];
osStaticThreadDef_t Task_RXControlBlock;
const osThreadAttr_t Task_RX_attributes = {
  .name = "Task_RX",
  .stack_mem = &Task_RXBuffer[0],
  .stack_size = sizeof(Task_RXBuffer),
  .cb_mem = &Task_RXControlBlock,
  .cb_size = sizeof(Task_RXControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_Supervisor */
osThreadId_t Task_SupervisorHandle;
uint32_t Task_SupervisorBuffer[ 512 ];
osStaticThreadDef_t Task_SupervisorControlBlock;
const osThreadAttr_t Task_Supervisor_attributes = {
  .name = "Task_Supervisor",
  .stack_mem = &Task_SupervisorBuffer[0],
  .stack_size = sizeof(Task_SupervisorBuffer),
  .cb_mem = &Task_SupervisorControlBlock,
  .cb_size = sizeof(Task_SupervisorControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_Control */
osThreadId_t Task_ControlHandle;
uint32_t Task_ControlBuffer[ 512 ];
osStaticThreadDef_t Task_ControlControlBlock;
const osThreadAttr_t Task_Control_attributes = {
  .name = "Task_Control",
  .stack_mem = &Task_ControlBuffer[0],
  .stack_size = sizeof(Task_ControlBuffer),
  .cb_mem = &Task_ControlControlBlock,
  .cb_size = sizeof(Task_ControlControlBlock),
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task_Debug */
osThreadId_t Task_DebugHandle;
uint32_t Task_DebugBuffer[ 512 ];
osStaticThreadDef_t Task_DebugControlBlock;
const osThreadAttr_t Task_Debug_attributes = {
  .name = "Task_Debug",
  .stack_mem = &Task_DebugBuffer[0],
  .stack_size = sizeof(Task_DebugBuffer),
  .cb_mem = &Task_DebugControlBlock,
  .cb_size = sizeof(Task_DebugControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_Led */
osThreadId_t Task_LedHandle;
uint32_t ledTaskBuffer[ 128 ];
osStaticThreadDef_t ledTaskControlBlock;
const osThreadAttr_t Task_Led_attributes = {
  .name = "Task_Led",
  .stack_mem = &ledTaskBuffer[0],
  .stack_size = sizeof(ledTaskBuffer),
  .cb_mem = &ledTaskControlBlock,
  .cb_size = sizeof(ledTaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mutex_Encoders */
osMutexId_t mutex_EncodersHandle;
osStaticMutexDef_t mutex_EncodersControlBlock;
const osMutexAttr_t mutex_Encoders_attributes = {
  .name = "mutex_Encoders",
  .cb_mem = &mutex_EncodersControlBlock,
  .cb_size = sizeof(mutex_EncodersControlBlock),
};
/* Definitions for mutex_BoardHealth */
osMutexId_t mutex_BoardHealthHandle;
osStaticMutexDef_t mutex_BoardHealthControlBlock;
const osMutexAttr_t mutex_BoardHealth_attributes = {
  .name = "mutex_BoardHealth",
  .cb_mem = &mutex_BoardHealthControlBlock,
  .cb_size = sizeof(mutex_BoardHealthControlBlock),
};
/* Definitions for mutex_UartRx */
osMutexId_t mutex_UartRxHandle;
osStaticMutexDef_t mutex_UartRxControlBlock;
const osMutexAttr_t mutex_UartRx_attributes = {
  .name = "mutex_UartRx",
  .cb_mem = &mutex_UartRxControlBlock,
  .cb_size = sizeof(mutex_UartRxControlBlock),
};
/* Definitions for mutex_Supervisor */
osMutexId_t mutex_SupervisorHandle;
osStaticMutexDef_t mutex_SupervisorControlBlock;
const osMutexAttr_t mutex_Supervisor_attributes = {
  .name = "mutex_Supervisor",
  .cb_mem = &mutex_SupervisorControlBlock,
  .cb_size = sizeof(mutex_SupervisorControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask_BattTemp(void *argument);
void StartTask_TX(void *argument);
void StartTask_RX(void *argument);
void StartTask_Supervisor(void *argument);
void StartTask_Control(void *argument);
void StartTask_Debug(void *argument);
void StartTask_Led(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */

    taskDISABLE_INTERRUPTS();

    // breakpoint qui
    for (;;);
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutex_Encoders */
  mutex_EncodersHandle = osMutexNew(&mutex_Encoders_attributes);

  /* creation of mutex_BoardHealth */
  mutex_BoardHealthHandle = osMutexNew(&mutex_BoardHealth_attributes);

  /* creation of mutex_UartRx */
  mutex_UartRxHandle = osMutexNew(&mutex_UartRx_attributes);

  /* creation of mutex_Supervisor */
  mutex_SupervisorHandle = osMutexNew(&mutex_Supervisor_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  EncoderSnapshot_MutexInit(mutex_EncodersHandle);
  BoardHealthSnapshot_MutexInit(mutex_BoardHealthHandle);
  RxSnapshot_MutexInit(mutex_UartRxHandle);
  SupervisorSnapshot_MutexInit(mutex_SupervisorHandle);
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Task_BattTemp */
  Task_BattTempHandle = osThreadNew(StartTask_BattTemp, NULL, &Task_BattTemp_attributes);

  /* creation of Task_TX */
  Task_TXHandle = osThreadNew(StartTask_TX, NULL, &Task_TX_attributes);

  /* creation of Task_RX */
  Task_RXHandle = osThreadNew(StartTask_RX, NULL, &Task_RX_attributes);

  /* creation of Task_Supervisor */
  Task_SupervisorHandle = osThreadNew(StartTask_Supervisor, NULL, &Task_Supervisor_attributes);

  /* creation of Task_Control */
  Task_ControlHandle = osThreadNew(StartTask_Control, NULL, &Task_Control_attributes);

  /* creation of Task_Debug */
  Task_DebugHandle = osThreadNew(StartTask_Debug, NULL, &Task_Debug_attributes);

  /* creation of Task_Led */
  Task_LedHandle = osThreadNew(StartTask_Led, NULL, &Task_Led_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_BattTemp */
/**
* @brief Function implementing the Task_BattTemp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_BattTemp */
void StartTask_BattTemp(void *argument)
{
  /* USER CODE BEGIN StartTask_BattTemp */
  /* Infinite loop */


	TickType_t lastWakeTime = osKernelGetTickCount();
	BoardHealth_TaskInit();
  for(;;)
  {
      uint32_t start, cycles;

      start = DWT->CYCCNT;

	  BoardHealth_TaskStep();

      cycles = DWT->CYCCNT - start;
      cycles_batt = cycles;
      if (cycles > wcet_batt_cycles_max){
          wcet_batt_cycles_max = cycles;
      }
      indice ++ ;

      free_words_batt = uxTaskGetStackHighWaterMark(NULL);

	  lastWakeTime += 100;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_BattTemp */
}

/* USER CODE BEGIN Header_StartTask_TX */
/**
* @brief Function implementing the Task_TX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_TX */
void StartTask_TX(void *argument)
{
  /* USER CODE BEGIN StartTask_TX */
	  TickType_t lastWakeTime = osKernelGetTickCount();


	  for (;;)
	  {
		uint32_t s = DWT->CYCCNT;

	    Tx_TaskStep();

	    WCET_Update(WCET_TASK_TX, DWT->CYCCNT - s);

	    free_words_tx = uxTaskGetStackHighWaterMark(NULL);

	    lastWakeTime += 20;   // es. 20 ms
	    osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_TX */
}

/* USER CODE BEGIN Header_StartTask_RX */
/**
* @brief Function implementing the Task_RX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_RX */
void StartTask_RX(void *argument)
{
  /* USER CODE BEGIN StartTask_RX */
	Rx_TaskInit();

  /* Infinite loop */
  for(;;)
  {
	  Rx_TaskStep();
  }
  /* USER CODE END StartTask_RX */
}

/* USER CODE BEGIN Header_StartTask_Supervisor */
/**
* @brief Function implementing the Task_Supervisor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Supervisor */
void StartTask_Supervisor(void *argument)
{
  /* USER CODE BEGIN StartTask_Supervisor */
	TickType_t lastWakeTime = osKernelGetTickCount();

	Supervisor_TaskInit();
  /* Infinite loop */
  for(;;)
  {

	  Supervisor_TaskStep();
	  lastWakeTime += 20;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_Supervisor */
}

/* USER CODE BEGIN Header_StartTask_Control */
/**
* @brief Function implementing the Task_Control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Control */
void StartTask_Control(void *argument)
{
  /* USER CODE BEGIN StartTask_Control */
	  TickType_t lastWakeTime = osKernelGetTickCount();
	  Encoder_Init();
	  Control_Init();
	  Actuation_Init();

	  static float last_cycle_cmd[4] = {0};
	  for (;;)
	  {
		  uint32_t start, cycles;
		  start = DWT->CYCCNT;

		  Encoder_Step(last_cycle_cmd);
		  ControlOutput_t new_cmd = Control_Step();
		  Actuation_Step(new_cmd);

		  last_cycle_cmd[0] = new_cmd.u_sx_a;
		  last_cycle_cmd[1] = new_cmd.u_dx_a;
		  last_cycle_cmd[2] = new_cmd.u_sx_p;
		  last_cycle_cmd[3] = new_cmd.u_dx_p;


		  cycles = DWT->CYCCNT - start;
		  cycles_ctrl = cycles;
		  if (cycles > wcet_ctrl_cycles_max){
			  wcet_ctrl_cycles_max = cycles;
		  }

		  free_words_ctrl = uxTaskGetStackHighWaterMark(NULL);

		  lastWakeTime += 10;          /* 10 ms */
		  osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_Control */
}

/* USER CODE BEGIN Header_StartTask_Debug */
/**
* @brief Function implementing the Task_Debug thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Debug */
void StartTask_Debug(void *argument)
{
  /* USER CODE BEGIN StartTask_Debug */
  /* Infinite loop */
	  TickType_t lastWakeTime = osKernelGetTickCount();
	  for(;;)
	  {
		  Log_TaskStep();

		  lastWakeTime += 1000;
		  osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_Debug */
}

/* USER CODE BEGIN Header_StartTask_Led */
/**
* @brief Function implementing the Task_Led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Led */
void StartTask_Led(void *argument)
{
  /* USER CODE BEGIN StartTask_Led */
	TickType_t lastWakeTime = osKernelGetTickCount();
  /* Infinite loop */
  for(;;)
  {

	  Led_TaskStep();
	  lastWakeTime += 100;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_Led */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


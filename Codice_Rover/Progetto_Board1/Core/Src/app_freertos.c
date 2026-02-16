/* USER CODE BEGIN Header */
/**
 * @file app_freertos.c
 * @brief Codice per le applicazioni FreeRTOS e la gestione dei task di sistema.
 * @details Questo file contiene l'inizializzazione del kernel FreeRTOS, la creazione
 * di mutex, thread e la definizione della logica di esecuzione dei task per il
 * controllo, la comunicazione e la diagnostica del sistema.
 * @copyright Copyright (c) 2026 STMicroelectronics. All rights reserved.
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/** @brief Periodo task default [ms]. */
#define TASK_DEFAULT_PERIOD_MS      100000U
/** @brief Periodo task BoardHealth [ms]. */
#define TASK_BATTTEMP_PERIOD_MS     100U
/** @brief Periodo task TX [ms]. */
#define TASK_TX_PERIOD_MS           20U
/** @brief Periodo task Supervisor [ms]. */
#define TASK_SUPERVISOR_PERIOD_MS   20U
/** @brief Periodo task Control [ms]. */
#define TASK_CONTROL_PERIOD_MS      10U
/** @brief Periodo task Debug/Log [ms]. */
#define TASK_DEBUG_PERIOD_MS        1000U
/** @brief Periodo task LED [ms]. */
#define TASK_LED_PERIOD_MS          100U
/** @brief Numero comandi ruota salvati tra un ciclo e il successivo. */
#define CONTROL_CMD_COUNT           4U
/** @brief Indice comando anteriore sinistro. */
#define CONTROL_CMD_IDX_SX_A        0U
/** @brief Indice comando anteriore destro. */
#define CONTROL_CMD_IDX_DX_A        1U
/** @brief Indice comando posteriore sinistro. */
#define CONTROL_CMD_IDX_SX_P        2U
/** @brief Indice comando posteriore destro. */
#define CONTROL_CMD_IDX_DX_P        3U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
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

/**
 * @brief Inizializzazione di FreeRTOS.
 * @note Questa funzione viene chiamata nel main.
 */
void MX_FREERTOS_Init(void);

/**
 * @brief Hook per la gestione dello Stack Overflow.
 * @param xTask Handle del task che ha causato l'overflow.
 * @param pcTaskName Nome del task che ha causato l'overflow.
 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */

    taskDISABLE_INTERRUPTS();

    for (;;);
}
/* USER CODE END 4 */

/**
  * @brief  Inizializzazione di Mutex, Thread e Snapshot.
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
 * @brief Task di default (Idle o basso consumo).
 * @param argument Non usato.
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    (void)argument;
  /* Infinite loop */
  for(;;)
  {
	  osDelay(TASK_DEFAULT_PERIOD_MS);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_BattTemp */
/**
 * @brief Task per il monitoraggio dello stato batteria e temperatura.
 * @details Esegue il campionamento dei sensori e aggiorna lo snapshot della board health.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_BattTemp */
void StartTask_BattTemp(void *argument)
{
  /* USER CODE BEGIN StartTask_BattTemp */
    (void)argument;
  /* Infinite loop */

  BoardHealth_TaskInit();
	TickType_t lastWakeTime = osKernelGetTickCount();
	
  for(;;)
  {

	  BoardHealth_TaskStep();

	  lastWakeTime += TASK_BATTTEMP_PERIOD_MS;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_BattTemp */
}

/* USER CODE BEGIN Header_StartTask_TX */
/**
 * @brief Task per la trasmissione dei dati via UART/Bus.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_TX */
void StartTask_TX(void *argument)
{
  /* USER CODE BEGIN StartTask_TX */
    (void)argument;
	  TickType_t lastWakeTime = osKernelGetTickCount();

	  for (;;)
	  {

	    Tx_TaskStep();

	    lastWakeTime += TASK_TX_PERIOD_MS;
	    osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_TX */
}

/* USER CODE BEGIN Header_StartTask_RX */
/**
 * @brief Task per la ricezione e il parsing dei dati in ingresso.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_RX */
void StartTask_RX(void *argument)
{
  /* USER CODE BEGIN StartTask_RX */
    (void)argument;
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
 * @brief Task del Supervisore di sistema.
 * @details Gestisce la logica degli stati e la sicurezza globale della board.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_Supervisor */
void StartTask_Supervisor(void *argument)
{
  /* USER CODE BEGIN StartTask_Supervisor */
    (void)argument;
  Supervisor_TaskInit();
	TickType_t lastWakeTime = osKernelGetTickCount();


  /* Infinite loop */
  for(;;)
  {

	  Supervisor_TaskStep();
	  lastWakeTime += TASK_SUPERVISOR_PERIOD_MS;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_Supervisor */
}

/* USER CODE BEGIN Header_StartTask_Control */
/**
 * @brief Task di Controllo Motori e Attuazione.
 * @details Legge gli encoder, esegue le leggi di controllo e invia i segnali agli attuatori.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_Control */
void StartTask_Control(void *argument)
{
  /* USER CODE BEGIN StartTask_Control */
    (void)argument;

    Encoder_Init();
    Control_Init();
    Actuation_Init();
    static float last_cycle_cmd[CONTROL_CMD_COUNT] = {0.0f};

    TickType_t lastWakeTime = osKernelGetTickCount();

	  for (;;)
	   {

		static SupervisorSnapshot_t sup;
		SupervisorSnapshot_Read(&sup);
		const bool actuation_enabled = sup.isBoardActuating;

		Encoder_Step(last_cycle_cmd);
		ControlOutput_t cmd = {0.0f};
		if (actuation_enabled != false)
        {
            cmd = Control_Step();
            Actuation_Step(cmd);

            last_cycle_cmd[CONTROL_CMD_IDX_SX_A] = cmd.u_sx_a;
            last_cycle_cmd[CONTROL_CMD_IDX_DX_A] = cmd.u_dx_a;
            last_cycle_cmd[CONTROL_CMD_IDX_SX_P] = cmd.u_sx_p;
            last_cycle_cmd[CONTROL_CMD_IDX_DX_P] = cmd.u_dx_p;
		}
        else
        {
            /* === reset PID === */
        	ControlLaw_Init();


            /* === board 1 riceve alcuna info da b2, quindi non conosce il comando dato ai motori === */
            last_cycle_cmd[CONTROL_CMD_IDX_SX_A] = 0.0f;
            last_cycle_cmd[CONTROL_CMD_IDX_DX_A] = 0.0f;
            last_cycle_cmd[CONTROL_CMD_IDX_SX_P] = 0.0f;
            last_cycle_cmd[CONTROL_CMD_IDX_DX_P] = 0.0f;
        }

	    lastWakeTime += TASK_CONTROL_PERIOD_MS;
	    osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_Control */
}

/* USER CODE BEGIN Header_StartTask_Debug */
/**
 * @brief Task di Debug e Logging.
 * @details Formatta e trasmette i log di sistema a intervalli regolari.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_Debug */
void StartTask_Debug(void *argument)
{
  /* USER CODE BEGIN StartTask_Debug */
    (void)argument;
  /* Infinite loop */
	  TickType_t lastWakeTime = osKernelGetTickCount();
	  for(;;)
	  {
		  Log_TaskStep();

		  lastWakeTime += TASK_DEBUG_PERIOD_MS;
		  osDelayUntil(lastWakeTime);
	  }
  /* USER CODE END StartTask_Debug */
}

/* USER CODE BEGIN Header_StartTask_Led */
/**
 * @brief Task per la gestione dei LED di segnalazione.
 * @param argument Non usato.
 */
/* USER CODE END Header_StartTask_Led */
void StartTask_Led(void *argument)
{
  /* USER CODE BEGIN StartTask_Led */
    (void)argument;
  Led_TaskInit();
	TickType_t lastWakeTime = osKernelGetTickCount();
  /* Infinite loop */
  for(;;)
  {

	  Led_TaskStep();
	  lastWakeTime += TASK_LED_PERIOD_MS;
	  osDelayUntil(lastWakeTime);
  }
  /* USER CODE END StartTask_Led */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


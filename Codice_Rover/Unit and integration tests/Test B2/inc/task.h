#ifndef TASK_H
#define TASK_H

#include "FreeRTOS.h"

// Funzioni usate nel tuo sonar_task.c e sonar_hw.c
TaskHandle_t xTaskGetCurrentTaskHandle(void);

// Mock per la notifica (ulTaskNotifyTake)
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);

// Mock per la notifica da ISR (xTaskNotifyFromISR)
BaseType_t xTaskNotifyFromISR(TaskHandle_t xTaskToNotify,
                             uint32_t ulValue,
                             eNotifyAction eAction,
                             BaseType_t *pxHigherPriorityTaskWoken);

// Macro per simulare lo yield
#define portYIELD_FROM_ISR( x ) (void)(x)

// Conversione millisecondi -> Ticks
#define pdMS_TO_TICKS( xTimeInMs ) ( (TickType_t) (xTimeInMs) )

#endif

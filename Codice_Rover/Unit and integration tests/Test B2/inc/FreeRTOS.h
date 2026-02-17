#ifndef FREERTOS_H
#define FREERTOS_H

#include <stdint.h>
#include <stddef.h>

typedef void* TaskHandle_t;
typedef uint32_t TickType_t;
typedef long BaseType_t;

#define pdFALSE 0
#define pdTRUE  1
#define pdPASS  1
#define portMAX_DELAY   (uint32_t)0xFFFFFFFF


typedef enum {
    eNoAction = 0,
    eSetBits,
    eIncrement,
    eSetValueWithOverwrite,
    eSetValueWithoutOverwrite
} eNotifyAction;

uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);
void vTaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken);
#endif

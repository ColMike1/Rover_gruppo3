#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sonar_snapshot.h"
#include "sonar_hw.h"
#include <string.h>
#include <stdbool.h>

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
SonarSnapshot_t captured_sonar_snap;
bool simulate_timeout = false;
extern sonars_flag_t flag;
extern buffer_sonars_t buffers;

int HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel) {
    return 0;
}

int HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length) {
    return 0;
}

int HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel) {
    return 0;
}

int HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel) {
    return 0;
}

BaseType_t xTaskNotifyFromISR(TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, BaseType_t *pxHigherPriorityTaskWoken) {
    if(pxHigherPriorityTaskWoken) *pxHigherPriorityTaskWoken = 0;
    return 1;
}

uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait) {
    if (xTicksToWait == 0) return 0;

    if (simulate_timeout) {
        return 0;
    }

    flag.sonar1_ok = 1;
    flag.sonar2_ok = 1;
    flag.sonar3_ok = 1;

    buffers.buf_ch1[0] = 1000;
    buffers.buf_ch1[1] = 1580;

    buffers.buf_ch2[0] = 1000;
    buffers.buf_ch2[1] = 2160;

    buffers.buf_ch3[0] = 1000;
    buffers.buf_ch3[1] = 2740;

    return 1;
}

TaskHandle_t xTaskGetCurrentTaskHandle(void) {
    return (TaskHandle_t)0x1234;
}

void SonarSnapshot_Write(SonarSnapshot_t *snap) {
    if(snap) memcpy(&captured_sonar_snap, snap, sizeof(SonarSnapshot_t));
}

void SonarSnapshot_Read(SonarSnapshot_t* dest) {
    if (dest != NULL) {
        memcpy(dest, &captured_sonar_snap, sizeof(SonarSnapshot_t));
    }
}

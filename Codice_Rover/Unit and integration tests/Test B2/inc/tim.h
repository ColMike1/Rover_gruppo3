#ifndef TIM_H
#define TIM_H

#include <stdint.h>

#define TIM1 ((void*)0x40010000)

typedef struct {
    void* Instance;
    uint32_t Channel;
} TIM_HandleTypeDef;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define TIM_CHANNEL_1 0x01
#define TIM_CHANNEL_2 0x02
#define TIM_CHANNEL_3 0x04

#define HAL_TIM_ACTIVE_CHANNEL_1 0x01
#define HAL_TIM_ACTIVE_CHANNEL_2 0x02
#define HAL_TIM_ACTIVE_CHANNEL_3 0x04

#define TIM_IT_CC1 0x01
#define TIM_IT_CC2 0x02
#define TIM_IT_CC3 0x04

#define __HAL_TIM_DISABLE_IT(handle, interrupt) (void)0
#define __HAL_TIM_CLEAR_IT(handle, interrupt)   (void)0

int HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
int HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
int HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
int HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

#endif

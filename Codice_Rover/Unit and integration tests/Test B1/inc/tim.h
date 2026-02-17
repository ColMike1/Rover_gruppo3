#ifndef TIM_H
#define TIM_H

#include <stdint.h>

#define TIM_CHANNEL_ALL 0xFFFFU

typedef struct
{
    uint32_t Instance;
} TIM_HandleTypeDef;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

uint32_t __HAL_TIM_GET_COUNTER(TIM_HandleTypeDef *htim);
int HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel);

#endif

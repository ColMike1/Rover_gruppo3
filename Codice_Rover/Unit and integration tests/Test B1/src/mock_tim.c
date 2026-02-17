#include "tim.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

uint32_t fake_timer_counter[4] = {0};

uint32_t __HAL_TIM_GET_COUNTER(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) return fake_timer_counter[0];
    if (htim == &htim4) return fake_timer_counter[1];
    if (htim == &htim1) return fake_timer_counter[2];
    if (htim == &htim3) return fake_timer_counter[3];
    return 0;
}
int HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    return 0;
}

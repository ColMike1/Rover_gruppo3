#include "cmsis_os2.h"

uint32_t fake_tick = 1000;

uint32_t osKernelGetTickCount(void)
{
    return fake_tick;
}

void osDelay(uint32_t ms) {}

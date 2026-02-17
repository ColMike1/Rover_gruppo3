#include "cmsis_os2.h"

uint32_t mock_tick = 0;

uint32_t osKernelGetTickCount(void) {
    return mock_tick;
}

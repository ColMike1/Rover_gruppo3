#ifndef CMSIS_OS2_H
#define CMSIS_OS2_H

#include <stdint.h>

typedef void* osMutexId_t;
typedef void* osThreadId_t;
typedef void* osEventFlagsId_t;

uint32_t osKernelGetTickCount(void);
void osDelay(uint32_t ms);

#endif

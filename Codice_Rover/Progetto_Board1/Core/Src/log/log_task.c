/*
 * log_task.c
 *
 *  Created on: Jan 19, 2026
 *      Author: Sterm
 */


#include "log/log_task.h"
#include "log/log_format.h"

#include "snapshot/encoder_snapshot.h"
#include "snapshot/board_health_snapshot.h"

#include "cmsis_os2.h"
#include <stdio.h>
#include "usart.h"

#include "log/wcet_monitor.h"

#define LOG_BUF_LEN          512

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&hlpuart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


void Log_TaskStep(void)
{
    static char log_buf[LOG_BUF_LEN];

    EncoderSnapshot_t enc;
    BoardHealthSnapshot_t bh;
    RxSnapshot_t rx;

    EncoderSnapshot_Read(&enc);
    BoardHealthSnapshot_Read(&bh);
    RxSnapshot_Read(&rx);

    Log_FormatSnapshot(log_buf, LOG_BUF_LEN,
                       &enc, &bh, &rx);

    printf("%s", log_buf);
    WCET_Print();
}

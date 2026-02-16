/**
 * @file log_task.c
 * @brief Task di logging periodico.
 * @details Estrae gli snapshot dai vari moduli e li invia sulla LPUART per il monitoraggio esterno.
 */

#include "log/log_task.h"
#include "log/log_format.h"
#include "snapshot/encoder_snapshot.h"
#include "snapshot/board_health_snapshot.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "usart.h"
#include "log/wcet_monitor.h"

/** @brief Dimensione del buffer locale per la riga di log. */
#define LOG_BUF_LEN 512

/**
 * @brief Override della funzione _write per reindirizzare printf su UART.
 */
int _write(int file, char *ptr, int len)
{
    /* Rule 11.4: Cast a uint8_t* per l'interfaccia HAL */
    (void)HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
    return len;
}

/**
 * @brief Esegue la raccolta dati e la stampa del report di sistema e dei tempi WCET.
 */
void Log_TaskStep(void)
{
    static char log_buf[LOG_BUF_LEN];
    EncoderSnapshot_t enc;
    BoardHealthSnapshot_t bh;
    RxSnapshot_t rx;

    EncoderSnapshot_Read(&enc);
    BoardHealthSnapshot_Read(&bh);
    RxSnapshot_Read(&rx);

    Log_FormatSnapshot(log_buf, (unsigned int)LOG_BUF_LEN, &enc, &bh, &rx);

    (void)printf("%s", log_buf);
    WCET_Print();
}

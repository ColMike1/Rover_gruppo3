/**
 * @file log_task.c
 * @brief Task di logging periodico su UART debug.
 */

#include "log/log_task.h"
#include "log/log_format.h"

#include "snapshot/ble_controller_snapshot.h"
#include "snapshot/imu_snapshot.h"
#include "snapshot/sonar_snapshot.h"

#include "cmsis_os2.h"
#include <stdio.h>
#include "usart.h"

#include "log/wcet_monitor.h"

/** @brief Dimensione del buffer locale usato per il log formattato. */
#define LOG_BUF_LEN          512U
/** @brief Dimensione del buffer locale usato per il report WCET. */
#define WCET_BUF_LEN         512U

/**
 * @brief Retarget di 'write' della libreria 'stdio.h' verso UART debug.
 * @param file Descriptor file (non usato).
 * @param ptr Buffer da trasmettere.
 * @param len Numero di byte da trasmettere.
 * @return Numero di byte trasmessi.
 */
int _write(int file, char *ptr, int len)
{
    (void)file;
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/**
 * @brief Esegue uno step del task di logging.
 *
 * Legge gli snapshot correnti, costruisce le stringhe diagnostiche e le invia
 * su UART debug.
 */
void Log_TaskStep(void)
{
    static char log_buf[LOG_BUF_LEN];
    static char wcet_buf[WCET_BUF_LEN];

    BleControllerSnapshot_t ble;
    IMUSnapshot_t imu;
    SonarSnapshot_t sonar;
    RxSnapshot_t rx;

    BleControllerSnapshot_Read(&ble);
    IMUSnapshot_Read(&imu);
    SonarSnapshot_Read(&sonar);
    RxSnapshot_Read(&rx);

    /* Formattazione separata dalla stampa: piu' riusabile e modificabile. */
    Log_FormatSnapshot(log_buf, LOG_BUF_LEN, &ble, &imu, &sonar, &rx);
    WCET_Format(wcet_buf, WCET_BUF_LEN);

    /* Nota MISRA: uso di printf solo per logging/debug. In release safety-critical puo' essere sostituito/rimosso. */
    printf("%s", log_buf);
    printf("%s", wcet_buf);
}

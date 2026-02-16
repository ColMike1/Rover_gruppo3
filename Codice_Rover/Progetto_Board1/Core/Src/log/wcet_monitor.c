/**
 * @file wcet_monitor.c
 * @brief Monitoraggio dei tempi di esecuzione nel peggior caso (Worst Case Execution Time).
 * @details Registra i cicli CPU massimi impiegati da task e ISR, convertendoli in microsecondi per l'analisi real-time.
 */

#include "stm32g4xx.h"
#include "log/wcet_monitor.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/** @brief Array statico per memorizzare i picchi massimi di cicli per ogni ID monitorato. */
static uint32_t wcet_max[WCET_ID_MAX];

/**
 * @brief Aggiorna il valore massimo di WCET per un determinato modulo.
 * @param id Identificativo del task o dell'ISR.
 * @param cycles Cicli CPU misurati.
 */
void WCET_Update(wcet_id_t id, uint32_t cycles)
{
    if ((id < WCET_ID_MAX) && (cycles > wcet_max[id]))
    {
        wcet_max[id] = cycles;
    }
}

/**
 * @brief Stampa su console seriale i tempi WCET registrati, raggruppati per tipologia.
 * @details Converte i cicli in microsecondi utilizzando la frequenza di clock di sistema.
 */
void WCET_Print(void)
{
    const double cyc2us = 1000000.0 / (double)SystemCoreClock;

    /* ================= TASK WCET ================= */
    (void)printf("WCET TASK | ENCODER=%lu (%.2fus) B_HEALTH=%lu (%.2fus) CONTROL=%lu (%.2fus) ACTUATION=%lu (%.2fus) TX=%lu (%.2fus) Mutex=%lu (%.2fus)\r\n",
        (unsigned long)wcet_max[WCET_TASK_ENCODER], (double)wcet_max[WCET_TASK_ENCODER] * cyc2us,
        (unsigned long)wcet_max[WCET_TASK_BOARD_HEALTH], (double)wcet_max[WCET_TASK_BOARD_HEALTH] * cyc2us,
        (unsigned long)wcet_max[WCET_TASK_CONTROL], (double)wcet_max[WCET_TASK_CONTROL] * cyc2us,
        (unsigned long)wcet_max[WCET_TASK_ACTUATION], (double)wcet_max[WCET_TASK_ACTUATION] * cyc2us,
        (unsigned long)wcet_max[WCET_TASK_TX], (double)wcet_max[WCET_TASK_TX] * cyc2us,
        (unsigned long)wcet_max[WCET_MUTEX], (double)wcet_max[WCET_MUTEX] * cyc2us);

    /* ================= ISR WCET ================= */
    (void)printf("WCET ISR | UART4=%lu (%.2fus) UART5=%lu (%.2fus) DMA1=%lu (%.2fus) DMA2=%lu (%.2fus) DMA3=%lu (%.2fus)\r\n",
        (unsigned long)wcet_max[WCET_ISR_UART_4], (double)wcet_max[WCET_ISR_UART_4] * cyc2us,
        (unsigned long)wcet_max[WCET_ISR_UART_5], (double)wcet_max[WCET_ISR_UART_5] * cyc2us,
        (unsigned long)wcet_max[WCET_ISR_DMA_1], (double)wcet_max[WCET_ISR_DMA_1] * cyc2us,
        (unsigned long)wcet_max[WCET_ISR_DMA_2], (double)wcet_max[WCET_ISR_DMA_2] * cyc2us,
        (unsigned long)wcet_max[WCET_ISR_DMA_3], (double)wcet_max[WCET_ISR_DMA_3] * cyc2us);

    /* ================= COSTO DELLE FUNZIONALITÀ ================= */
    (void)printf("WCET FEATURE | ENCODER=%lu (%.2fus) B_HEALTH_TOT=%lu (%.2fus) CONTROL_RX=%lu (%.2fus)\r\n\n",
        (unsigned long)wcet_max[WCET_TASK_ENCODER], (double)wcet_max[WCET_TASK_ENCODER] * cyc2us,
        (unsigned long)(wcet_max[WCET_TASK_BOARD_HEALTH] + wcet_max[WCET_ISR_DMA_1]),
        (double)(wcet_max[WCET_TASK_BOARD_HEALTH] + wcet_max[WCET_ISR_DMA_1]) * cyc2us,
        (unsigned long)(wcet_max[WCET_TASK_CONTROL]), (double)wcet_max[WCET_TASK_CONTROL] * cyc2us);
}

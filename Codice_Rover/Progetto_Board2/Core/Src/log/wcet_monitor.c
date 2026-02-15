/**
 * @file wcet_monitor.c
 * @brief Monitor semplice del Worst-Case Execution Time (WCET).
 */

#include "stm32g4xx.h"
#include "log/wcet_monitor.h"
#include <stdio.h>

/** @brief Macro utility per calcolo massimo tra due valori. */
#define WCET_MAX(a,b)  (((a) > (b)) ? (a) : (b))

/** @brief Massimo numero di cicli osservato per ciascun ID monitorato. */
static uint32_t wcet_max[WCET_ID_MAX];

/**
 * @brief Aggiorna il WCET di un task/ISR monitorato.
 * @param id Identificativo della voce WCET.
 * @param cycles Numero di cicli misurati.
 */
void WCET_Update(wcet_id_t id, uint32_t cycles)
{
    if (id >= WCET_ID_MAX)
    {
        return;
    }

    if (cycles > wcet_max[id])
    {
        wcet_max[id] = cycles;
    }
}

/**
 * @brief Formatta in testo il riepilogo WCET.
 * @param buf Buffer di output.
 * @param buf_len Lunghezza buffer di output.
 */
void WCET_Format(char *buf, uint32_t buf_len)
{
    if ((buf == NULL) || (buf_len == 0U))
    {
        return;
    }

    const double cyc2us = 1e6 / (double)SystemCoreClock;
    size_t offset = 0U;
    size_t rem = (size_t)buf_len;
    int ret;

    buf[0] = '\0';

    /* Nota MISRA: uso di snprintf solo per logging/debug. In release safety-critical puo' essere sostituito/rimosso. */
    ret = snprintf(&buf[offset], rem,
        "WCET TASK |"
        "BLE=%lu (%.2fus) "
        "IMU=%lu (%.2fus) "
        "SONAR=%lu (%.2fus)\r\n"
        "SUPERVISOR=%lu (%.2fus)\r\n",
        wcet_max[WCET_TASK_BLE],
        wcet_max[WCET_TASK_BLE] * cyc2us,
        wcet_max[WCET_TASK_IMU],
        wcet_max[WCET_TASK_IMU] * cyc2us,
        wcet_max[WCET_TASK_SONAR],
        wcet_max[WCET_TASK_SONAR] * cyc2us,
        wcet_max[WCET_TASK_SUPERVISOR],
        wcet_max[WCET_TASK_SUPERVISOR] * cyc2us
    );
    if (ret < 0) { buf[0] = '\0'; return; }
    if ((size_t)ret >= rem) { return; }
    offset += (size_t)ret;
    rem -= (size_t)ret;

    ret = snprintf(&buf[offset], rem,
        "WCET ISR I2C |"
        "I2C1_EV=%lu (%.2fus) "
        "I2C1_ER=%lu (%.2fus) "
        "I2C3_EV=%lu (%.2fus) "
        "I2C3_ER=%lu (%.2fus)\r\n",
        wcet_max[WCET_ISR_I2C1_EV],
        wcet_max[WCET_ISR_I2C1_EV] * cyc2us,
        wcet_max[WCET_ISR_I2C1_ER],
        wcet_max[WCET_ISR_I2C1_ER] * cyc2us,
        wcet_max[WCET_ISR_I2C3_EV],
        wcet_max[WCET_ISR_I2C3_EV] * cyc2us,
        wcet_max[WCET_ISR_I2C3_ER],
        wcet_max[WCET_ISR_I2C3_ER] * cyc2us
    );
    if (ret < 0) { buf[0] = '\0'; return; }
    if ((size_t)ret >= rem) { return; }
    offset += (size_t)ret;
    rem -= (size_t)ret;

    ret = snprintf(&buf[offset], rem,
        "WCET ISR DMA |"
        "DMA1=%lu (%.2fus) "
        "DMA2=%lu (%.2fus) "
        "DMA3=%lu (%.2fus) "
        "DMA4=%lu (%.2fus)\r\n",
        wcet_max[WCET_ISR_DMA_1],
        wcet_max[WCET_ISR_DMA_1] * cyc2us,
        wcet_max[WCET_ISR_DMA_2],
        wcet_max[WCET_ISR_DMA_2] * cyc2us,
        wcet_max[WCET_ISR_DMA_3],
        wcet_max[WCET_ISR_DMA_3] * cyc2us,
        wcet_max[WCET_ISR_DMA_4],
        wcet_max[WCET_ISR_DMA_4] * cyc2us
    );
    if (ret < 0) { buf[0] = '\0'; return; }
    if ((size_t)ret >= rem) { return; }
    offset += (size_t)ret;
    rem -= (size_t)ret;

    ret = snprintf(&buf[offset], rem,
        "WCET ISR TIM |"
        "TIM_IC=%lu (%.2fus)\r\n",
        wcet_max[WCET_ISR_TIM_IC],
        wcet_max[WCET_ISR_TIM_IC] * cyc2us
    );
    if (ret < 0) { buf[0] = '\0'; return; }
    if ((size_t)ret >= rem) { return; }
    offset += (size_t)ret;
    rem -= (size_t)ret;

    ret = snprintf(&buf[offset], rem,
        "WCET FEATURE CPU |"
        "BLE=%lu (%.2fus) "
        "IMU=%lu (%.2fus) "
        "SONAR=%lu (%.2fus) "
        "RX =%lu (%.2fus) "
        "TX =%lu (%.2fus)\r\n\n\n",

        wcet_max[WCET_TASK_BLE] + WCET_MAX(wcet_max[WCET_ISR_I2C1_EV], wcet_max[WCET_ISR_I2C1_ER]),
        (wcet_max[WCET_TASK_BLE] + WCET_MAX(wcet_max[WCET_ISR_I2C1_EV], wcet_max[WCET_ISR_I2C1_ER])) * cyc2us,
        wcet_max[WCET_TASK_IMU] + WCET_MAX(wcet_max[WCET_ISR_I2C3_EV], wcet_max[WCET_ISR_I2C3_ER]),
        (wcet_max[WCET_TASK_IMU] + WCET_MAX(wcet_max[WCET_ISR_I2C3_EV], wcet_max[WCET_ISR_I2C3_ER])) * cyc2us,
        wcet_max[WCET_TASK_SONAR] + wcet_max[WCET_ISR_DMA_1] + wcet_max[WCET_ISR_DMA_2] + wcet_max[WCET_ISR_DMA_3],
        (wcet_max[WCET_TASK_SONAR] + wcet_max[WCET_ISR_DMA_1] + wcet_max[WCET_ISR_DMA_2] + wcet_max[WCET_ISR_DMA_3]) * cyc2us,

        wcet_max[WCET_TASK_RX],
        wcet_max[WCET_TASK_RX] * cyc2us,

        wcet_max[WCET_TASK_TX] + wcet_max[WCET_ISR_DMA_4],
        (wcet_max[WCET_TASK_TX] + wcet_max[WCET_ISR_DMA_4]) * cyc2us
    );
    if (ret < 0) { buf[0] = '\0'; }
}

/**
 * @brief Stampa a console il riepilogo WCET.
 */
void WCET_Print(void)
{
    char wcet_buf[512U];

    WCET_Format(wcet_buf, (uint32_t)sizeof(wcet_buf));

    /* Nota MISRA: uso di printf solo per logging/debug. In release safety-critical puo' essere sostituito/rimosso. */
    printf("%s", wcet_buf);
}


/**
 * @file wcet_monitor.h
 * @brief API monitor WCET e formattazione report diagnostico.
 */

#ifndef INC_LOG_WCET_MONITOR_H_
#define INC_LOG_WCET_MONITOR_H_

#include <stdint.h>

typedef enum {
    WCET_TASK_SONAR,
    WCET_TASK_IMU,
    WCET_TASK_BLE,
	WCET_TASK_TX,
	WCET_TASK_RX,
	WCET_TASK_SUPERVISOR,

    WCET_ISR_I2C1_EV,
    WCET_ISR_I2C1_ER,

    WCET_ISR_I2C3_EV,
    WCET_ISR_I2C3_ER,

	WCET_ISR_DMA_1,
	WCET_ISR_DMA_2,
	WCET_ISR_DMA_3,
	WCET_ISR_DMA_4,
	WCET_ISR_DMA_5,

    WCET_ISR_TIM_IC,

    WCET_ID_MAX
} wcet_id_t;

/**
 * @brief Aggiorna il WCET per un ID monitorato.
 * @param id Identificativo task/ISR.
 * @param cycles Numero di cicli rilevato.
 */
void WCET_Update(wcet_id_t id, uint32_t cycles);

/**
 * @brief Formatta il report WCET in un buffer testuale.
 * @param buf Buffer di output.
 * @param buf_len Lunghezza del buffer.
 */
void WCET_Format(char *buf, uint32_t buf_len);

/**
 * @brief Stampa il report WCET (API legacy, solo debug).
 */
void WCET_Print(void);


#endif /* INC_LOG_WCET_MONITOR_H_ */

/*
 * wcet_monitor.h
 *
 *  Created on: Jan 21, 2026
 *      Author: Sterm
 */

#ifndef INC_LOG_WCET_MONITOR_H_
#define INC_LOG_WCET_MONITOR_H_

#include <stdint.h>

typedef enum {
    WCET_TASK_ENCODER,
    WCET_TASK_BOARD_HEALTH,
	WCET_TASK_TX,
	WCET_TASK_RX,
	WCET_TASK_SUPERVISOR,
	WCET_TASK_CONTROL,
	WCET_TASK_ACTUATION,
	WCET_MUTEX,

    WCET_ISR_UART_4,
    WCET_ISR_UART_5,

	WCET_ISR_DMA_1,
	WCET_ISR_DMA_2,
	WCET_ISR_DMA_3,

    WCET_ID_MAX
} wcet_id_t;

void WCET_Update(wcet_id_t id, uint32_t cycles);
void WCET_Print(void);


#endif /* INC_LOG_WCET_MONITOR_H_ */

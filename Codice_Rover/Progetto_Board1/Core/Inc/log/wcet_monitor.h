/**
 * @file wcet_monitor.h
 * @brief Monitoraggio del "Worst Case Execution Time" (WCET) per task e ISR.
 */

#ifndef INC_LOG_WCET_MONITOR_H_
#define INC_LOG_WCET_MONITOR_H_

#include <stdint.h>

/**
 * @enum wcet_id_t
 * @brief Identificativi univoci per i task e le ISR monitorati.
 */
typedef enum {
    WCET_TASK_ENCODER,      /**< Task gestione encoder */
    WCET_TASK_BOARD_HEALTH, /**< Task monitoraggio hardware */
	WCET_TASK_TX,           /**< Task trasmissione */
	WCET_TASK_RX,           /**< Task ricezione */
	WCET_TASK_SUPERVISOR,   /**< Task supervisore di sistema */
	WCET_TASK_CONTROL,      /**< Task legge di controllo */
	WCET_TASK_ACTUATION,    /**< Task attuazione motori */
	WCET_MUTEX,             /**< Monitoraggio tempi di attesa Mutex */

    WCET_ISR_UART_4,        /**< Interrupt Service Routine UART 4 */
    WCET_ISR_UART_5,        /**< Interrupt Service Routine UART 5 */

	WCET_ISR_DMA_1,         /**< Interrupt Service Routine DMA 1 */
	WCET_ISR_DMA_2,         /**< Interrupt Service Routine DMA 2 */
	WCET_ISR_DMA_3,         /**< Interrupt Service Routine DMA 3 */

    WCET_ID_MAX             /**< Limite superiore per l'array di monitoraggio */
} wcet_id_t;

/**
 * @brief Aggiorna il tempo di esecuzione registrato per un determinato ID.
 * @param id Identificativo della risorsa.
 * @param cycles Numero di cicli di clock impiegati.
 */
void WCET_Update(wcet_id_t id, uint32_t cycles);

/**
 * @brief Stampa o logga i risultati del monitoraggio WCET.
 */
void WCET_Print(void);

#endif /* INC_LOG_WCET_MONITOR_H_ */

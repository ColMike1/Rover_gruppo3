/**
 * @file comm_rx_task.h
 * @brief Gestione del task di ricezione dei messaggi.
 */

#ifndef INC_COMM_COMM_RX_TASK_H_
#define INC_COMM_COMM_RX_TASK_H_

/**
 * @brief Inizializza le risorse necessarie per il task di ricezione (code, semafori, etc.).
 */
void Rx_TaskInit(void);

/**
 * @brief Esegue un passo del task di ricezione, elaborando i dati in ingresso dalla UART.
 */
void Rx_TaskStep(void);

#endif /* INC_COMM_COMM_RX_TASK_H_ */

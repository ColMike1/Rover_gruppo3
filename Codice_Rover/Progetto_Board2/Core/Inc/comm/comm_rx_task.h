/**
 * @file comm_rx_task.h
 * @brief Task di ricezione UART e parsing frame da board 1.
 */

#ifndef INC_COMM_COMM_RX_TASK_H_
#define INC_COMM_COMM_RX_TASK_H_

/**
 * @brief Inizializza il task RX.
 */
void Rx_TaskInit(void);

/**
 * @brief Esegue uno step del task RX.
 */
void Rx_TaskStep(void);

#endif /* INC_COMM_COMM_RX_TASK_H_ */

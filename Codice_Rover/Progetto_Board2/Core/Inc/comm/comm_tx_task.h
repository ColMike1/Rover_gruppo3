/**
 * @file comm_tx_task.h
 * @brief Task di trasmissione frame da board 2 verso board 1.
 */

#ifndef INC_COMM_COMM_TX_TASK_H_
#define INC_COMM_COMM_TX_TASK_H_

/**
 * @brief Inizializza il task TX.
 */
void Tx_TaskInit(void);

/**
 * @brief Esegue uno step del task TX.
 */
void Tx_TaskStep(void);

#endif /* INC_COMM_COMM_TX_TASK_H_ */

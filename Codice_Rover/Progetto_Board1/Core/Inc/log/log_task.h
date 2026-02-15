/**
 * @file log_task.h
 * @brief Gestione del task periodico di logging del sistema.
 */

#ifndef INC_LOG_LOG_TASK_H_
#define INC_LOG_LOG_TASK_H_

/**
 * @brief Inizializza il sistema di logging (es. periferiche SD o UART dedicate).
 */
void Log_TaskInit(void);

/**
 * @brief Raccoglie i dati dagli snapshot e li invia al supporto di memorizzazione.
 */
void Log_TaskStep(void);

#endif /* INC_LOG_LOG_TASK_H_ */

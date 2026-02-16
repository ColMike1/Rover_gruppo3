/**
 * @file log_task.h
 * @brief API del task di logging periodico.
 */

#ifndef INC_LOG_LOG_TASK_H_
#define INC_LOG_LOG_TASK_H_

/**
 * @brief Inizializza il task di logging.
 */
void Log_TaskInit(void);

/**
 * @brief Esegue uno step del task di logging.
 */
void Log_TaskStep(void);


#endif /* INC_LOG_LOG_TASK_H_ */

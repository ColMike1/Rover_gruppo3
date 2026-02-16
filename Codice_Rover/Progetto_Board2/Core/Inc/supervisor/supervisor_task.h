/**
 * @file supervisor_task.h
 * @brief API del task supervisore periodico.
 */

#ifndef INC_SUPERVISOR_SUPERVISOR_TASK_H_
#define INC_SUPERVISOR_SUPERVISOR_TASK_H_

/**
 * @brief Inizializza il task supervisore.
 */
void Supervisor_TaskInit(void);

/**
 * @brief Esegue uno step del task supervisore.
 */
void Supervisor_TaskStep(void);

#endif /* INC_SUPERVISOR_SUPERVISOR_TASK_H_ */

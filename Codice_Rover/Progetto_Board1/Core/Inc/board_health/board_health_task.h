/**
 * @file board_health_task.h
 * @brief Interfaccia per il task di monitoraggio di sistema.
 */

#ifndef INC_BOARD_HEALTH_BOARD_HEALTH_TASK_H_
#define INC_BOARD_HEALTH_BOARD_HEALTH_TASK_H_

/**
 * @brief Inizializza il task o le risorse per il monitoraggio board health.
 */
void BoardHealth_TaskInit(void);

/**
 * @brief Esegue ciclicamente il monitoraggio dei parametri vitali.
 */
void BoardHealth_TaskStep(void);

#endif /* INC_BOARD_HEALTH_BOARD_HEALTH_TASK_H_ */

/**
 * @file supervisor_task.h
 * @brief Gestione del task supervisore di sistema.
 * * Questo modulo si occupa della logica di alto livello, della gestione dei fault
 * e della transizione degli stati operativi del robot.
 */

#ifndef INC_SUPERVISOR_SUPERVISOR_TASK_H_
#define INC_SUPERVISOR_SUPERVISOR_TASK_H_

/**
 * @brief Inizializza il task supervisore, i timer e le strutture di stato interne.
 */
void Supervisor_TaskInit(void);

/**
 * @brief Esegue un ciclo della logica di supervisione.
 * * Analizza gli snapshot di sistema, valuta i fault attivi e decide
 * i riferimenti di velocità e sterzata autorizzati per il controllo.
 */
void Supervisor_TaskStep(void);

#endif /* INC_SUPERVISOR_SUPERVISOR_TASK_H_ */

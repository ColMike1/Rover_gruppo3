/**
 * @file sonar_task.h
 * @brief API del task sonar periodico.
 */

#ifndef INC_SONAR_SONAR_TASK_H_
#define INC_SONAR_SONAR_TASK_H_

/**
 * @brief Inizializza il task sonar.
 */
void Sonar_TaskInit(void);

/**
 * @brief Esegue uno step del task sonar.
 */
void Sonar_TaskStep(void);


#endif /* INC_SONAR_SONAR_TASK_H_ */

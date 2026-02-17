/**
 * @file led_task.h
 * @brief Gestione del task per la segnalazione visiva tramite LED.
 */

#ifndef INC_LED_LED_TASK_H_
#define INC_LED_LED_TASK_H_

/**
 * @brief Inizializza l'hardware e le risorse per il task LED.
 */
void Led_TaskInit();

/**
 * @brief Esegue un ciclo del task LED (es. lampeggio di stato).
 */
void Led_TaskStep();

#endif /* INC_LED_LED_TASK_H_ */

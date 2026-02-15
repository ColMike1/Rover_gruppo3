/**
 * @file control_step.h
 * @brief Interfaccia per il loop di controllo di alto livello.
 */

#ifndef INC_CONTROL_CONTROL_STEP_H_
#define INC_CONTROL_CONTROL_STEP_H_

#include "control/control_types.h"

/**
 * @brief Inizializza il modulo di controllo e le relative variabili di stato.
 */
void Control_Init(void);

/**
 * @brief Esegue l'iterazione principale del controllo.
 * @return ControlOutput_t I comandi calcolati pronti per essere inviati all'attuazione.
 */
ControlOutput_t Control_Step(void);

#endif /* INC_CONTROL_CONTROL_STEP_H_ */

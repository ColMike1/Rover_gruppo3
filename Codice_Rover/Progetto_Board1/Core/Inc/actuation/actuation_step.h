/**
 * @file actuation_step.h
 * @brief Gestione del singolo step di attuazione del sistema.
 */

#ifndef INC_ACTUATION_ACTUATION_STEP_H_
#define INC_ACTUATION_ACTUATION_STEP_H_

#include "control/control_law.h"

/**
 * @brief Inizializza il modulo di attuazione.
 */
void Actuation_Init(void);

/**
 * @brief Esegue un singolo passo di attuazione basato sull'output del controllo.
 * @param ctrl_out Struttura contenente i comandi generati dalla legge di controllo.
 */
void Actuation_Step(ControlOutput_t ctrl_out);

#endif /* INC_ACTUATION_ACTUATION_STEP_H_ */

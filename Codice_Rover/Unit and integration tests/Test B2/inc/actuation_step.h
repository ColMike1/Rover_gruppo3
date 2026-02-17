/**
 * @file actuation_step.h
 * @brief Interfaccia dello step di attuazione board 2.
 */

#ifndef INC_ACTUATION_ACTUATION_STEP_H_
#define INC_ACTUATION_ACTUATION_STEP_H_

/**
 * @brief Inizializza il modulo di attuazione.
 */
void Actuation_Init(void);

/**
 * @brief Esegue uno step di attuazione open-loop.
 * @param v_ref Riferimento lineare normalizzato.
 * @param omega_ref Riferimento angolare normalizzato.
 */
void Actuation_Step(float v_ref, float omega_ref);

#endif /* INC_ACTUATION_ACTUATION_STEP_H_ */

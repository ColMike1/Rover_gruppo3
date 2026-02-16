/**
 * @file control_law.h
 * @brief Implementazione dell'algoritmo della legge di controllo (PID, logica di sterzata).
 */

#ifndef INC_CONTROL_CONTROL_LAW_H_
#define INC_CONTROL_CONTROL_LAW_H_

#include <stdint.h>
#include "control/control_types.h"

/**
 * @brief Inizializzazione della legge di controllo.
 * * Esegue il reset dei regolatori PID, degli stati interni e dei filtri.
 */
void ControlLaw_Init(void);

/**
 * @brief Esegue il calcolo della legge di controllo.
 * @param[in]  in  Puntatore alla struttura con gli input (riferimenti e feedback).
 * @param[out] out Puntatore alla struttura dove verranno scritti i comandi calcolati.
 */
void ControlLaw_Step(const ControlInput_t *in, ControlOutput_t *out);


#endif /* INC_CONTROL_CONTROL_LAW_H_ */

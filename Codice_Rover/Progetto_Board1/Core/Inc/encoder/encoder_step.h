/**
 * @file encoder_step.h
 * @brief Gestione dell'elaborazione periodica dei dati encoder.
 */

#ifndef INC_ENCODER_ENCODER_STEP_H_
#define INC_ENCODER_ENCODER_STEP_H_

/**
 * @brief Inizializza i timer e le periferiche dedicate agli encoder.
 */
void Encoder_Init(void);

/**
 * @brief Esegue il campionamento degli encoder e converte i tick in unità fisiche.
 * @param[out] speeds Array di 4 float dove verranno scritte le velocità calcolate.
 */
void Encoder_Step(float speeds[4]);

#endif /* INC_ENCODER_ENCODER_STEP_H_ */

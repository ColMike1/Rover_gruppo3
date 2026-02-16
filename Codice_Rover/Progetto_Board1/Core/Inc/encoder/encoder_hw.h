/**
 * @file encoder_hw.h
 * @brief Driver di basso livello per l'interfacciamento con gli encoder hardware.
 */

#ifndef INC_ENCODER_ENCODER_HW_H_
#define INC_ENCODER_ENCODER_HW_H_

#include <stdint.h>

/**
 * @brief Stati di ritorno per le operazioni sugli encoder.
 */
typedef enum
{
  ENCODER_OK = 0,      /**< Lettura eseguita correttamente */
  ENCODER_OVERFLOW     /**< Rilevato overflow del contatore hardware */
} EncoderStatus_t;

/**
 * @brief Legge la variazione di tick (delta) con gestione dell'overflow.
 * @param encoder_id Identificativo dell'encoder hardware.
 * @param[out] delta_ticks Puntatore alla variabile che conterrà i tick accumulati (con segno).
 * @return EncoderStatus_t Esito della lettura.
 */
EncoderStatus_t Encoder_ReadDelta(uint8_t encoder_id, int *delta_ticks);

#endif /* INC_ENCODER_ENCODER_HW_H_ */

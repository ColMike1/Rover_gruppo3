/*
 * encoder_hw.h
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */

#ifndef INC_ENCODER_ENCODER_HW_H_
#define INC_ENCODER_ENCODER_HW_H_

#include <stdint.h>

typedef enum
{
  ENCODER_OK = 0,
  ENCODER_OVERFLOW
} EncoderStatus_t;

/**
 * @brief Read delta ticks with overflow handling
 * @param encoder_id Encoder index
 * @param delta_ticks Signed delta ticks
 */
EncoderStatus_t Encoder_ReadDelta(uint8_t encoder_id, int32_t *delta_ticks);

#endif /* INC_ENCODER_ENCODER_HW_H_ */

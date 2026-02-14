/*
 * board_health_adc.h
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */

#ifndef INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_
#define INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_


#include <stdint.h>


typedef enum
{
  BOARD_HEALTH_OK = 0,
  BOARD_HEALTH_ADC_ERROR,
  BOARD_HEALTH_OUT_OF_RANGE
} BoardHealthStatus_t;

/**
 * @brief Read MCU internal temperature sensor
 * @param[out] temp_degC Temperature in degrees Celsius
 * @return BoardHealthStatus_t
 */
BoardHealthStatus_t BoardHealth_ReadTemperature(float *temp_degC);

/**
 * @brief Read battery level
 * @param[out] battery_pct Battery level in percentage [0..100]
 * @return BoardHealthStatus_t
 */
BoardHealthStatus_t BoardHealth_ReadBattery(float *battery_pct);


void BoardHealthADC_Init(void);


#endif /* INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_ */

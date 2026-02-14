/*
 * ble_controller_i2c.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */


#include "ble_controller/ble_controller_i2c.h"
#include <stdbool.h>
/* Config I2C */
#define BLE_I2C_TIMEOUT_MS   5
#define BLE_I2C_ADDRESS     (0x55 << 1)


/* I2C handle from CubeMX */
extern I2C_HandleTypeDef hi2c1;



BleI2CStatus_t BleController_I2C_ReadFrame(BleRawFrame_t *frame)
{
    if (!frame)
    	return BLE_I2C_ERROR;

    HAL_StatusTypeDef st = HAL_I2C_Master_Receive(
        &hi2c1,
        BLE_I2C_ADDRESS,
        (uint8_t *)frame,
        sizeof(BleRawFrame_t),
        BLE_I2C_TIMEOUT_MS
    );

    return st;
}



/**
 * @file ble_controller_i2c.c
 * @brief Implementazione accesso I2C al controller BLE.
 */


#include "ble_controller/ble_controller_i2c.h"

/** @brief Timeout massimo della transazione I2C [ms]. */
#define BLE_I2C_TIMEOUT_MS   5
/** @brief Indirizzo I2C del controller BLE (formato HAL, 7 bit shiftato). */
#define BLE_I2C_ADDRESS     (0x55 << 1)


/** @brief Handle I2C1 generato da CubeMX. */
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Legge in polling un frame completo dal controller BLE.
 * @param frame Puntatore al frame di output.
 * @return Stato dell'operazione I2C.
 */
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



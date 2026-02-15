/**
 * @file ble_controller_i2c.h
 * @brief Interfaccia I2C per lettura frame dal controller BLE.
 */

#ifndef INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_
#define INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/* ================= STATUS ================= */

/** @brief Esito delle operazioni I2C del modulo BLE. */
typedef enum
{
    BLE_I2C_COMPLETE = 0,
    BLE_I2C_ERROR,
	BLE_I2C_BUSY,
    BLE_I2C_TIMEOUT
} BleI2CStatus_t;


/** @brief Frame raw ricevuto dal controller BLE. */
typedef struct __attribute__((packed))
{
    uint16_t ax;
    uint16_t ay;
    uint8_t  a_btn;

    uint16_t bx;
    uint16_t by;
    uint8_t  b_btn;

    uint8_t  btn1;
    uint8_t  btn2;

} BleRawFrame_t;



/**
 * @brief Legge un frame completo dal controller BLE.
 * @param frame Puntatore alla struttura di output.
 * @return Stato dell'operazione I2C.
 */
BleI2CStatus_t BleController_I2C_ReadFrame(BleRawFrame_t *frame);



#endif /* INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_ */

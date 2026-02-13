/*
 * ble_controller_i2c.h
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */

#ifndef INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_
#define INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/* ================= STATUS ================= */

typedef enum
{
    BLE_I2C_COMPLETE = 0,
    BLE_I2C_ERROR,
	BLE_I2C_BUSY,
    BLE_I2C_TIMEOUT
} BleI2CStatus_t;


/* ================= FRAME ================= */

/* Frame ricevuto via BLE (protocollo fisso) */
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



void BleController_I2C_OnRxComplete(void);
void BleController_I2C_OnError(void);

/* ================= API ================= */



/* Ricezione frame completo */
BleI2CStatus_t BleController_I2C_ReadFrame(BleRawFrame_t *frame);



#endif /* INC_BLE_CONTROLLER_BLE_CONTROLLER_I2C_H_ */

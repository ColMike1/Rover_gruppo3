/*
 * ble_controller_task.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */


#include "ble_controller/ble_controller_task.h"
#include "ble_controller/ble_controller_i2c.h"
#include "snapshot/ble_controller_snapshot.h"
#include "cmsis_os2.h"
#include <math.h>

/* ===== CONFIG ===== */

#define JOY_CENTER     255.0f
#define JOY_SCALE      255.0f
#define JOY_DEADZONE   0.1f



static float NormalizeAxis(uint16_t raw)
{
    float v = ((float)raw - JOY_CENTER) / JOY_SCALE;

    if (fabsf(v) < JOY_DEADZONE)
        v = 0.0f;

    if (v > 1.0f)  v = 1.0f;
    if (v < -1.0f) v = -1.0f;

    return v;
}

/* ===== API ===== */



void BleController_TaskStep(void)
{
    static BleControllerSnapshot_t snap;
    BleRawFrame_t frame;

    uint32_t now = osKernelGetTickCount();
    snap.task_last_run_ms = now;

    BleI2CStatus_t st = BleController_I2C_ReadFrame(&frame);

    if (st == BLE_I2C_COMPLETE)
    {
        snap.data_last_valid_ms = now;

        /* Normalizzazione assi joystick A */
        snap.ax_norm = NormalizeAxis(frame.ax);
        snap.ay_norm = NormalizeAxis(frame.ay);

        snap.bx_norm = NormalizeAxis(frame.bx);
        snap.by_norm = NormalizeAxis(frame.by);


        /* Pulsanti */
        snap.a_btn = frame.a_btn;
        snap.b_btn = frame.b_btn;
        snap.btn1  = frame.btn1;
        snap.btn2  = frame.btn2;
    }

    BleControllerSnapshot_Write(&snap);
}

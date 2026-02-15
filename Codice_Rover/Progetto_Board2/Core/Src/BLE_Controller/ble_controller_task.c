/**
 * @file ble_controller_task.c
 * @brief Task di acquisizione e normalizzazione dati del controller BLE.
 */


#include "ble_controller/ble_controller_task.h"
#include "ble_controller/ble_controller_i2c.h"
#include "snapshot/ble_controller_snapshot.h"
#include "cmsis_os2.h"
#include <math.h>

/** @brief Centro joystick in unita' raw. */
#define JOY_CENTER     255.0f
/** @brief Fattore di scala raw -> valore normalizzato. */
#define JOY_SCALE      255.0f
/** @brief Deadzone applicata ai joystick normalizzati. */
#define JOY_DEADZONE   0.1f

/**
 * @brief Normalizza un asse joystick da raw a intervallo [-1, 1].
 * @param raw Valore raw letto dal frame BLE.
 * @return Valore normalizzato con deadzone applicata.
 */
static float NormalizeAxis(uint16_t raw)
{
    float v = ((float)raw - JOY_CENTER) / JOY_SCALE;

    if (fabsf(v) < JOY_DEADZONE)
    {
        v = 0.0f;
    }

    if (v > 1.0f)
    {
        v = 1.0f;
    }
    if (v < -1.0f)
    {
        v = -1.0f;
    }

    return v;
}

/**
 * @brief Inizializza il task BLE.
 *
 * In questa implementazione non e' richiesta inizializzazione specifica.
 */
void BleController_TaskInit(void)
{
}

/**
 * @brief Esegue uno step del task BLE.
 *
 * Se la lettura I2C va a buon fine, aggiorna gli assi normalizzati e i pulsanti
 * nello snapshot condiviso.
 */
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

        /* Normalizzazione assi joystick */
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

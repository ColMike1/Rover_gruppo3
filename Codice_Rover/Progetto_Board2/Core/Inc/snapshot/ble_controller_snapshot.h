/**
 * @file ble_controller_snapshot.h
 * @brief Definizione dello snapshot BLE controller e API thread-safe.
 */

#ifndef INC_SNAPSHOT_BLE_CONTROLLER_SNAPSHOT_H_
#define INC_SNAPSHOT_BLE_CONTROLLER_SNAPSHOT_H_

#include <stdint.h>
#include "ble_controller/ble_controller_i2c.h"

#include "cmsis_os.h"
/** @brief Snapshot dei dati acquisiti dal controller BLE. */
typedef struct
{
    uint32_t task_last_run_ms;     /* ultima esecuzione del task */
    uint32_t data_last_valid_ms;  /* ultimo istante in cui i dati sono validi */


    /* Stick A normalizzato */
    float ax_norm;
    float ay_norm;

    /* Stick B normalizzato */
    float bx_norm;
    float by_norm;

    /* Pulsanti */
    uint8_t a_btn;
    uint8_t b_btn;
    uint8_t btn1;
    uint8_t btn2;

} BleControllerSnapshot_t;


/**
 * @brief Registra il mutex usato per proteggere lo snapshot.
 * @param mutex_handle Handle del mutex creato all'esterno.
 */
void BleControllerSnapshot_MutexInit(osMutexId_t mutex_handle);

/**
 * @brief Scrive lo snapshot BLE in sezione critica.
 * @param snap Puntatore al dato sorgente da copiare.
 */
void BleControllerSnapshot_Write(const BleControllerSnapshot_t *snap);

/**
 * @brief Legge lo snapshot BLE in sezione critica.
 * @param snap Puntatore al buffer di destinazione.
 */
void BleControllerSnapshot_Read(BleControllerSnapshot_t *snap);

#endif /* INC_SNAPSHOT_BLE_CONTROLLER_SNAPSHOT_H_ */

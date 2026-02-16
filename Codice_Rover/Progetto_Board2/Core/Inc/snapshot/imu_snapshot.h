/**
 * @file imu_snapshot.h
 * @brief Definizione dello snapshot IMU e API thread-safe.
 */

#ifndef INC_SNAPSHOT_IMU_SNAPSHOT_H_
#define INC_SNAPSHOT_IMU_SNAPSHOT_H_

#include <stdint.h>
#include "IMU/imu_i2c.h"
#include "cmsis_os.h"

/** @brief Snapshot delle misure IMU e dati temporali. */
typedef struct
{

    uint32_t task_last_run_ms;     /* ultima esecuzione del task */
    uint32_t data_last_valid_ms;  /* ultimo istante in cui i dati sono validi */


    /* Accellerometro (g) */
    float ax_g;
    float ay_g;
    float az_g;

    /* Giroscopio (deg/s) */
    float gx_dps;
    float gy_dps;
    float gz_dps;

    /* Temperatura (°C) */
    float temperature_degC;

    float yaw;

} IMUSnapshot_t;


/**
 * @brief Registra il mutex usato per proteggere lo snapshot.
 * @param mutex_handle Handle del mutex creato all'esterno.
 */
void IMUSnapshot_MutexInit(osMutexId_t mutex_handle);

/**
 * @brief Scrive lo snapshot IMU in sezione critica.
 * @param snap Puntatore al dato sorgente da copiare.
 */
void IMUSnapshot_Write(const IMUSnapshot_t *snap);

/**
 * @brief Legge lo snapshot IMU in sezione critica.
 * @param snap Puntatore al buffer di destinazione.
 */
void IMUSnapshot_Read(IMUSnapshot_t *snap);

#endif /* INC_SNAPSHOT_IMU_SNAPSHOT_H_ */

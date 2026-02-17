/*
 * imu_snapshot.h
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */

#ifndef INC_SNAPSHOT_IMU_SNAPSHOT_H_
#define INC_SNAPSHOT_IMU_SNAPSHOT_H_

#include <stdint.h>
#include "imu_i2c.h"
#include "cmsis_os2.h"

/* ================= SNAPSHOT ================= */

typedef struct
{

    uint32_t task_last_run_ms;     /* ultima esecuzione del task */
    uint32_t data_last_valid_ms;  /* ultimo istante in cui i dati sono validi */


    /* Accelerometer (g) */
    float ax_g;
    float ay_g;
    float az_g;

    /* Gyroscope (deg/s) */
    float gx_dps;
    float gy_dps;
    float gz_dps;

    /* Temperature (°C) */
    float temperature_degC;

    float yaw;

} IMUSnapshot_t;


/* ================= API ================= */
void IMUSnapshot_MutexInit(osMutexId_t mutex_handle);
void IMUSnapshot_Write(const IMUSnapshot_t *snap);
void IMUSnapshot_Read(IMUSnapshot_t *snap);

#endif /* INC_SNAPSHOT_IMU_SNAPSHOT_H_ */

/**
 * @file imu_task.c
 * @brief Task di acquisizione IMU e pubblicazione snapshot.
 */

#include "IMU/imu.h"
#include "snapshot/imu_snapshot.h"

#include "cmsis_os2.h"

/** @brief Ritardo iniziale task IMU per stabilizzare le periferiche [ms]. */
#define IMU_TASK_INIT_DELAY_MS 100U

/**
 * @brief Inizializza il task IMU.
 */
void IMU_TaskInit(void)
{
    /* Piccolo ritardo per stabilizzazione della periferica IMU. */
    osDelay(IMU_TASK_INIT_DELAY_MS);
    IMU_I2C_Init();
}

/**
 * @brief Esegue uno step del task IMU.
 *
 * Legge i dati dal sensore, aggiorna i timestamp e pubblica lo snapshot.
 */
void IMU_TaskStep(void)
{
    static IMUSnapshot_t snap;

    /* Timestamp di esecuzione del task. */
    uint32_t now = osKernelGetTickCount();

    snap.task_last_run_ms = now;

    /* Aggiornamento dati sensore. */
    IMUI2CStatus_t st = IMU_I2C_ReadBlocking();

    if (st == IMU_I2C_COMPLETE)
    {
        /* Timestamp di produzione del dato valido. */
        snap.data_last_valid_ms = now;
        /* Accelerometro. */
        IMU_I2C_GetAccel(&snap.ax_g, &snap.ay_g, &snap.az_g);

        /* Giroscopio. */
        IMU_I2C_GetGyro(&snap.gx_dps, &snap.gy_dps, &snap.gz_dps);

        snap.yaw = IMU_I2C_GetYaw();

        /* Temperatura sensore. */
        snap.temperature_degC = IMU_I2C_GetTemperature();
    }

    /* Pubblicazione snapshot. */
    IMUSnapshot_Write(&snap);
}

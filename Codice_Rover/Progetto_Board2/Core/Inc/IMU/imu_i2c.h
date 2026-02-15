/**
 * @file imu_i2c.h
 * @brief API I2C per inizializzazione e lettura sensore MPU6050.
 */

#ifndef INC_IMU_IMU_I2C_H_
#define INC_IMU_IMU_I2C_H_

#include <stdint.h>

#include "i2c.h"

/** @brief Indirizzo I2C MPU6050 in formato 8 bit (7 bit shiftato). */
#define MPU6050_ADDR          (0x68U << 1U)

/** @brief Registro identificativo dispositivo. */
#define MPU6050_WHO_AM_I      0x75U
/** @brief Registro power management 1. */
#define MPU6050_PWR_MGMT_1    0x6BU
/** @brief Registro divisore sample rate. */
#define MPU6050_SMPLRT_DIV    0x19U
/** @brief Registro configurazione giroscopio. */
#define MPU6050_GYRO_CONFIG   0x1BU
/** @brief Registro configurazione accelerometro. */
#define MPU6050_ACCEL_CONFIG  0x1CU

/** @brief Primo registro burst accelerometro X high byte. */
#define MPU6050_ACCEL_XOUT_H  0x3BU
/** @brief Registro temperatura high byte. */
#define MPU6050_TEMP_OUT_H    0x41U
/** @brief Primo registro burst giroscopio X high byte. */
#define MPU6050_GYRO_XOUT_H   0x43U
/** @brief Registro giroscopio Z high byte. */
#define MPU6050_GYRO_ZOUT_H   0x47U

/** @brief Sensibilita' accelerometro per range +/-2g [LSB/g]. */
#define MPU6050_ACCEL_SENS_2G     16384.0f
/** @brief Sensibilita' giroscopio per range +/-250 dps [LSB/(deg/s)]. */
#define MPU6050_GYRO_SENS_250DPS  131.0f

/**
 * @brief Stato delle operazioni I2C del modulo IMU.
 */
typedef enum
{
    IMU_I2C_COMPLETE = 0,
    IMU_I2C_ERROR,
    IMU_I2C_BUSY,
    IMU_I2C_TIMEOUT
} IMUI2CStatus_t;

/**
 * @brief Inizializza e configura il sensore MPU6050.
 * @return Stato dell'inizializzazione.
 */
IMUI2CStatus_t IMU_I2C_Init(void);

/**
 * @brief Esegue una lettura bloccante dei registri dati sensore.
 * @return Stato della transazione I2C.
 */
IMUI2CStatus_t IMU_I2C_ReadBlocking(void);

/**
 * @brief Restituisce le accelerazioni correnti.
 * @param[out] ax_g Accelerazione asse X [g].
 * @param[out] ay_g Accelerazione asse Y [g].
 * @param[out] az_g Accelerazione asse Z [g].
 */
void IMU_I2C_GetAccel(float *ax_g, float *ay_g, float *az_g);

/**
 * @brief Restituisce le velocita' angolari correnti.
 * @param[out] gx_dps Velocita' asse X [deg/s].
 * @param[out] gy_dps Velocita' asse Y [deg/s].
 * @param[out] gz_dps Velocita' asse Z [deg/s].
 */
void IMU_I2C_GetGyro(float *gx_dps, float *gy_dps, float *gz_dps);

/**
 * @brief Restituisce la temperatura corrente del sensore.
 * @return Temperatura in gradi Celsius.
 */
float IMU_I2C_GetTemperature(void);

/**
 * @brief Restituisce la stima di yaw corrente.
 * @return Angolo yaw in gradi [0, 360].
 */
float IMU_I2C_GetYaw(void);

#endif /* INC_IMU_IMU_I2C_H_ */

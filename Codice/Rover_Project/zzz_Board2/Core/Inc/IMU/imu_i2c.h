/*
 * imu_i2c.h
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */

#ifndef INC_IMU_IMU_I2C_H_
#define INC_IMU_IMU_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/* ================= MPU6050 DEFINITIONS ================= */

#define MPU6050_ADDR          (0x68 << 1)

/* Registers */
#define MPU6050_WHO_AM_I      0x75
#define MPU6050_PWR_MGMT_1    0x6B
#define MPU6050_SMPLRT_DIV    0x19
#define MPU6050_GYRO_CONFIG   0x1B
#define MPU6050_ACCEL_CONFIG  0x1C

#define MPU6050_ACCEL_XOUT_H  0x3B
#define MPU6050_TEMP_OUT_H    0x41
#define MPU6050_GYRO_XOUT_H   0x43

#define MPU6050_GYRO_ZOUT_H   0x47


/* Sensitivity (±2g, ±250 dps) */
#define MPU6050_ACCEL_SENS_2G     16384.0f
#define MPU6050_GYRO_SENS_250DPS  131.0f


/* ================= STATUS ================= */

typedef enum
{
	IMU_I2C_COMPLETE = 0,
	IMU_I2C_ERROR,
	IMU_I2C_BUSY,
	IMU_I2C_TIMEOUT
} IMUI2CStatus_t;



/* ================= API ================= */

/* Init and configuration */
IMUI2CStatus_t IMU_I2C_Init(void);

IMUI2CStatus_t IMU_I2C_ReadBlocking(void);

/* Getters: values in physical units */
void IMU_I2C_GetAccel(float *ax_g, float *ay_g, float *az_g);
void IMU_I2C_GetGyro(float *gx_dps, float *gy_dps, float *gz_dps);
float IMU_I2C_GetTemperature(void);


float IMU_I2C_GetYaw(void);

#endif /* INC_IMU_IMU_I2C_H_ */


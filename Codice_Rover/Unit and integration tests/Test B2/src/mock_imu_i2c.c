#include "imu_i2c.h"

IMUI2CStatus_t imu_read_result = IMU_I2C_COMPLETE;

IMUI2CStatus_t IMU_I2C_ReadBlocking(void)
{
    return imu_read_result;
}

IMUI2CStatus_t IMU_I2C_Init(void)
{
    return IMU_I2C_COMPLETE;
}

void IMU_I2C_GetAccel(float *a, float *b, float *c) {}
void IMU_I2C_GetGyro(float *a, float *b, float *c) {}
float IMU_I2C_GetYaw(void) { return 42.0f; }
float IMU_I2C_GetTemperature(void) { return 25.0f; }

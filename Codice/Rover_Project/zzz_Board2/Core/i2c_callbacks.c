/*
 * i2c_callbacks.c
 *
 *  Created on: Jan 21, 2026
 *      Author: Sterm
 */


#include "ble_controller/ble_controller_i2c.h"
#include "IMU/imu_i2c.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c3)
    {
        IMU_I2C_OnRxComplete();
    }

}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c1)
    {
        BleController_I2C_OnRxComplete();
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c1)
    {
        BleController_I2C_OnError();
    }
    else if(hi2c == &hi2c3){
        IMU_I2C_OnError();
    }
}

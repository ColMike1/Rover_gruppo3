/**
 * @file imu_i2c.c
 * @brief Driver I2C per acquisizione dati da MPU6050.
 */

#include <math.h>
#include "IMU/imu_i2c.h"
#include "cmsis_os2.h"

/** @brief Numero di byte letti dal burst dati MPU6050. */
#define IMU_BURST_LEN_BYTES        14U
/** @brief Timeout standard per transazioni I2C [ms]. */
#define IMU_I2C_TIMEOUT_MS         5U
/** @brief Campioni usati per stimare il bias del gyro Z. */
#define IMU_GYRO_BIAS_SAMPLES      500U
/** @brief Delay tra campioni durante la stima bias [ms]. */
#define IMU_GYRO_BIAS_DELAY_MS     2U
/** @brief Deadband su gyro Z [dps]. */
#define IMU_YAW_DEADBAND_DPS       0.3f
/** @brief Periodo di aggiornamento yaw [s]. */
#define IMU_YAW_PERIOD_S           0.02f
/** @brief Convenzione segno gyro Z: yaw oraria positiva. */
#define IMU_GYRO_Z_SIGN            (-1.0f)
/** @brief Valore atteso del registro WHO_AM_I. */
#define IMU_WHO_AM_I_EXPECTED      0x70U
/** @brief Valore registro PWR_MGMT_1 per wake-up MPU6050. */
#define IMU_PWR_MGMT_1_WAKE_VAL     0x00U
/** @brief Valore SMPLRT_DIV per sample rate target. */
#define IMU_SMPLRT_DIV_VAL          0x07U
/** @brief Valore ACCEL_CONFIG per range +/-2g. */
#define IMU_ACCEL_CONFIG_2G_VAL     0x00U
/** @brief Valore GYRO_CONFIG per range +/-250 dps. */
#define IMU_GYRO_CONFIG_250_VAL     0x00U

/** @brief Dimensione indirizzo registro per HAL I2C Mem API. */
#define IMU_I2C_MEMADD_SIZE         1U

/** @brief Handle I2C3 generato da CubeMX. */
extern I2C_HandleTypeDef hi2c3;

/** @brief Buffer raw di lettura MPU6050 (14 byte consecutivi). */
static uint8_t imu_rx_buf[IMU_BURST_LEN_BYTES];

/** @brief Misure raw accelerometro. */
static int16_t accel_raw[3U];
/** @brief Misure raw giroscopio. */
static int16_t gyro_raw[3U];
/** @brief Misura raw temperatura. */
static int16_t temp_raw;

/** @brief Accelerazioni scalate in g. */
static float accel_g[3U];
/** @brief Velocita' angolari scalate in dps. */
static float gyro_dps[3U];
/** @brief Temperatura scalata in gradi Celsius. */
static float temperature_degC;

/** @brief Bias asse Z del giroscopio stimato in fase di init. */
static float gz_bias = 0.0f;
/** @brief Yaw integrata in gradi. */
static float yaw_deg = 0.0f;

/**
 * @brief Scrive un registro MPU6050 con gestione stato HAL.
 * @param reg Indirizzo registro.
 * @param value Valore da scrivere.
 * @return Stato IMU dell'operazione.
 */
static IMUI2CStatus_t IMU_I2C_WriteReg(uint16_t reg, uint8_t value)
{
    HAL_StatusTypeDef st = HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, reg, IMU_I2C_MEMADD_SIZE, &value, 1U, IMU_I2C_TIMEOUT_MS);

    if (st == HAL_OK)
    {
        return IMU_I2C_COMPLETE;
    }

    return IMU_I2C_ERROR;
}

/**
 * @brief Stima il bias del giroscopio Z mediando campioni a riposo.
 */
static void MPU6050_CalcGyroBias(void)
{
    float sum = 0.0f;
    uint8_t buf[2];

    for (uint32_t i = 0U; i < IMU_GYRO_BIAS_SAMPLES; i++)
    {
        if (HAL_I2C_Mem_Read(&hi2c3, MPU6050_ADDR, MPU6050_GYRO_ZOUT_H, 1U, buf, 2U, IMU_I2C_TIMEOUT_MS) == HAL_OK)
        {
            int16_t raw = (int16_t)(((uint16_t)buf[0] << 8U) | (uint16_t)buf[1]);
            sum += (float)raw / MPU6050_GYRO_SENS_250DPS;
        }
        osDelay(IMU_GYRO_BIAS_DELAY_MS);
    }

    gz_bias = sum / (float)IMU_GYRO_BIAS_SAMPLES;
}

/**
 * @brief Aggiorna la stima di yaw integrando la velocita' angolare Z.
 * @param Ts Tempo di campionamento in secondi.
 */
static void MPU6050_UpdateYaw(float Ts)
{
    float gz = gyro_dps[2U];

    /* Deadband anti-rumore. */
    if (fabsf(gz) > IMU_YAW_DEADBAND_DPS)
    {
        yaw_deg += gz * Ts;
    }

    if (yaw_deg >= 360.0f)
    {
        yaw_deg -= 360.0f;
    }
    if (yaw_deg < 0.0f)
    {
        yaw_deg += 360.0f;
    }
}

/**
 * @brief Converte il buffer raw MPU6050 in grandezze fisiche.
 */
static void IMU_I2C_Parse(void)
{
    const uint8_t *buf = imu_rx_buf;

    accel_raw[0U] = (int16_t)(((uint16_t)buf[0U] << 8U) | (uint16_t)buf[1U]);
    accel_raw[1U] = (int16_t)(((uint16_t)buf[2U] << 8U) | (uint16_t)buf[3U]);
    accel_raw[2U] = (int16_t)(((uint16_t)buf[4U] << 8U) | (uint16_t)buf[5U]);

    accel_g[0U] = accel_raw[0U] / MPU6050_ACCEL_SENS_2G;
    accel_g[1U] = accel_raw[1U] / MPU6050_ACCEL_SENS_2G;
    accel_g[2U] = accel_raw[2U] / MPU6050_ACCEL_SENS_2G;

    temp_raw = (int16_t)(((uint16_t)buf[6U] << 8U) | (uint16_t)buf[7]);
    temperature_degC = (temp_raw / 340.0f) + 36.53f;

    gyro_raw[0U] = (int16_t)(((uint16_t)buf[8U] << 8U) | (uint16_t)buf[9U]);
    gyro_raw[1U] = (int16_t)(((uint16_t)buf[10U] << 8U) | (uint16_t)buf[11U]);
    gyro_raw[2U] = (int16_t)(((uint16_t)buf[12U] << 8U) | (uint16_t)buf[13U]);

    gyro_dps[0U] = gyro_raw[0U] / MPU6050_GYRO_SENS_250DPS;
    gyro_dps[1U] = gyro_raw[1U] / MPU6050_GYRO_SENS_250DPS;
    
    /* Inversione asse Z per rispecchiare la convenzione: accellerazione positiva in rotazione oraria. */
    gyro_dps[2U] = IMU_GYRO_Z_SIGN * (gyro_raw[2U] / MPU6050_GYRO_SENS_250DPS - gz_bias);
}

/**
 * @brief Legge in polling i registri misura e aggiorna i dati interni.
 * @return Stato della transazione I2C.
 */
IMUI2CStatus_t IMU_I2C_ReadBlocking(void)
{
    HAL_StatusTypeDef st = HAL_I2C_Mem_Read(&hi2c3, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1U, imu_rx_buf, IMU_BURST_LEN_BYTES, IMU_I2C_TIMEOUT_MS);

    if (st == HAL_OK)
    {
        IMU_I2C_Parse();
        MPU6050_UpdateYaw(IMU_YAW_PERIOD_S);
    }

    return st;
}

/**
 * @brief Inizializza e configura l'MPU6050.
 * @return Stato dell'inizializzazione.
 */
IMUI2CStatus_t IMU_I2C_Init(void)
{
    uint8_t who_am_i = 0U;
    uint8_t data;

    if (HAL_I2C_Mem_Read(&hi2c3, MPU6050_ADDR, MPU6050_WHO_AM_I, IMU_I2C_MEMADD_SIZE, &who_am_i, 1U, IMU_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return IMU_I2C_ERROR;
    }

    /* Con questa configurazione hardware il valore atteso e' 0x70. */
    if (who_am_i != IMU_WHO_AM_I_EXPECTED)
    {
        return IMU_I2C_ERROR;
    }

    /* Wake-up dispositivo. */
    data = IMU_PWR_MGMT_1_WAKE_VAL;
    if (IMU_I2C_WriteReg(MPU6050_PWR_MGMT_1, data) != IMU_I2C_COMPLETE)
    {
        return IMU_I2C_ERROR;
    }

    /* Sample rate = 1 kHz. */
    data = IMU_SMPLRT_DIV_VAL;
    if (IMU_I2C_WriteReg(MPU6050_SMPLRT_DIV, data) != IMU_I2C_COMPLETE)
    {
        return IMU_I2C_ERROR;
    }

    /* Accelerometro: +/-2g. */
    data = IMU_ACCEL_CONFIG_2G_VAL;
    if (IMU_I2C_WriteReg(MPU6050_ACCEL_CONFIG, data) != IMU_I2C_COMPLETE)
    {
        return IMU_I2C_ERROR;
    }

    /* Giroscopio: +/-250 dps. */
    data = IMU_GYRO_CONFIG_250_VAL;
    if (IMU_I2C_WriteReg(MPU6050_GYRO_CONFIG, data) != IMU_I2C_COMPLETE)
    {
        return IMU_I2C_ERROR;
    }

    MPU6050_CalcGyroBias();

    return IMU_I2C_COMPLETE;
}

/**
 * @brief Restituisce le accelerazioni correnti.
 * @param ax_g Uscita asse X in g.
 * @param ay_g Uscita asse Y in g.
 * @param az_g Uscita asse Z in g.
 */
void IMU_I2C_GetAccel(float *ax_g, float *ay_g, float *az_g)
{
    if (ax_g != NULL)
    {
        *ax_g = accel_g[0U];
    }
    if (ay_g != NULL)
    {
        *ay_g = accel_g[1U];
    }
    if (az_g != NULL)
    {
        *az_g = accel_g[2U];
    }
}

/**
 * @brief Restituisce le velocita' angolari correnti.
 * @param gx_dps Uscita asse X in dps.
 * @param gy_dps Uscita asse Y in dps.
 * @param gz_dps Uscita asse Z in dps.
 */
void IMU_I2C_GetGyro(float *gx_dps, float *gy_dps, float *gz_dps)
{
    if (gx_dps != NULL)
    {
        *gx_dps = gyro_dps[0U];
    }
    if (gy_dps != NULL)
    {
        *gy_dps = gyro_dps[1U];
    }
    if (gz_dps != NULL)
    {
        *gz_dps = gyro_dps[2U];
    }
}

/**
 * @brief Restituisce la temperatura corrente del sensore.
 * @return Temperatura in gradi Celsius.
 */
float IMU_I2C_GetTemperature(void)
{
    return temperature_degC;
}

/**
 * @brief Restituisce la stima di yaw corrente.
 * @return Yaw in gradi [0, 360].
 */
float IMU_I2C_GetYaw(void)
{
    return yaw_deg;
}

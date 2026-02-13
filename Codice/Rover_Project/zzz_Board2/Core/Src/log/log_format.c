/*
 * log_format.c
 *
 *  Created on: Jan 19, 2026
 *      Author: Sterm
 */



#include <stdio.h>
#include <string.h>

#include "log/log_format.h"
#include "snapshot/supervisor_snapshot.h"

#include "shared_resources/board1_faults.h"
#include "shared_resources/board2_faults.h"
static const char *CommUnpackStatusToStr(CommUnpackStatus_t s)
{
    switch (s)
    {
        case COMM_UNPACK_OK:   return "OK";
        case COMM_UNPACK_NULL: return "NULL";
        case COMM_UNPACK_LEN:  return "LEN";
        case COMM_UNPACK_CRC:  return "CRC";
        default:               return "UNK";
    }
}

void B1FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if (!buf || len == 0) return;

    buf[0] = '\0';

    if (flags == FAULT_NONE) {
        strncpy(buf, "NONE", len);
        return;
    }
    if (flags & FAULT_TEMP) 	strncat(buf, "TEMP-",len);
    if (flags & FAULT_BATT) 	strncat(buf, "BATT-",len);
    if (flags & FAULT_RX) 		strncat(buf, "RX-",len);
    if (flags & FAULT_WHEEL_FL) strncat(buf, "FL-", len);
    if (flags & FAULT_WHEEL_FR) strncat(buf, "FR-", len);
    if (flags & FAULT_WHEEL_RL) strncat(buf, "RL-", len);
    if (flags & FAULT_WHEEL_RR) strncat(buf, "RR-", len);
    if (flags & FAULT_B2_SUP)   strncat(buf, "B2SUP-", len);

    size_t l = strlen(buf);
    if (l > 0) buf[l - 1] = '\0';
}


void B2FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if (!buf || len == 0) return;

    buf[0] = '\0';

    if (flags == FAULT_NONE) {
        strncpy(buf, "NONE", len);
        return;
    }

    if (flags & FAULT_BLE)     strncat(buf, "BLE-", len);
    if (flags & FAULT_IMU)     strncat(buf, "IMU-", len);
    if (flags & FAULT_RX)	   strncat(buf, "RX-", len);
    if (flags & FAULT_B1_SUP)  strncat(buf, "B1SUP-", len);

    size_t l = strlen(buf);
    if (l > 0) buf[l - 1] = '\0';
}


void Log_FormatSnapshot(char *buf,
                        unsigned buf_len,
                        const BleControllerSnapshot_t *ble,
                        const IMUSnapshot_t *imu,
                        const SonarSnapshot_t *sonar,
						const RxSnapshot_t *rx)
{
    if (!buf || !ble || !imu || !sonar || !rx)
        return;

    char b1_crit[64];
    char b1_degr[64];
    char b2_crit[64];
    char b2_degr[64];

    static SupervisorSnapshot_t sup;
    SupervisorSnapshot_Read(&sup);

    B1FaultFlagsToStr(rx->payload.critical_mask, b1_crit, sizeof(b1_crit));
    B1FaultFlagsToStr(rx->payload.degraded_mask, b1_degr, sizeof(b1_degr));
    B2FaultFlagsToStr(sup.critical_mask, b2_crit, sizeof(b2_crit));
    B2FaultFlagsToStr(sup.degraded_mask, b2_degr, sizeof(b2_degr));

    snprintf(buf, buf_len,
        "[SNAPSHOT]\r\n"
        "BLE   t=%lu | AX=%.2f AY=%.2f | BTN=%d%d%d%d (data time: %lu) \r\n"
        "IMU   t=%lu | ACC(%.2f %.2f %.2f) | GYR(%.2f %.2f %.2f) yaw: %.2f | T=%.1f (data time: %lu)\r\n"
        "SONAR t=%lu | F=%ucm (t: %lu) | L=%ucm (t: %lu) | R=%ucm (t: %lu) | \r\n"
        "RX t = %lu | last=%s | valid_t=%lu\r\n"
        "RX PAYLOAD | FL=%.2f FR=%.2f RL=%.2f RR=%.2f | criticalB1=%s | degradedB1=%s | alive=%u\r\n"
        "criticalB2=%s | degradedB2=%s\r\n\r\n",

        /* BLE */
        ble->task_last_run_ms,
        ble->bx_norm, ble->ay_norm,
        ble->a_btn, ble->b_btn, ble->btn1, ble->btn2,
        ble->data_last_valid_ms,
		//ble->i2c_status,

        /* IMU */
        imu->task_last_run_ms,
        imu->ax_g, imu->ay_g, imu->az_g,
        imu->gx_dps, imu->gy_dps, imu->gz_dps, imu->yaw,
        imu->temperature_degC,
        imu->data_last_valid_ms,

        /* SONAR */
        sonar->task_last_run_ms,
        sonar->dist_cm[0], sonar->data_last_valid_ms[0],
        sonar->dist_cm[1], sonar->data_last_valid_ms[1],
        sonar->dist_cm[2], sonar->data_last_valid_ms[2],
        //(sonar->valid_mask != 0U) ? "OK" : "INV",

        /* RX SNAPSHOT B1 */
        rx->task_last_run_ms,
        CommUnpackStatusToStr(rx->last_event),
        rx->data_last_valid_ms,

        rx->payload.wheel_speed_rpm[0],
        rx->payload.wheel_speed_rpm[1],
        rx->payload.wheel_speed_rpm[2],
        rx->payload.wheel_speed_rpm[3],
		b1_crit,
		b1_degr,
        rx->payload.alive_counter,
		b2_crit,
		b2_degr
    );

}


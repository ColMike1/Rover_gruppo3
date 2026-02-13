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
                        const EncoderSnapshot_t *enc,
                        const BoardHealthSnapshot_t *bh,
						const RxSnapshot_t *rx)
{
    if (!buf || !enc || !bh || !rx)
        return;

    char b1_crit[64];
    char b1_degr[64];
    char b2_crit[64];
    char b2_degr[64];

    static SupervisorSnapshot_t sup;
    SupervisorSnapshot_Read(&sup);

    B1FaultFlagsToStr(sup.critical_mask, b1_crit, sizeof(b1_crit));
    B1FaultFlagsToStr(sup.degraded_mask, b1_degr, sizeof(b1_degr));
    B2FaultFlagsToStr(rx->payload.critical_mask, b2_crit, sizeof(b2_crit));
    B2FaultFlagsToStr(rx->payload.degraded_mask, b2_degr, sizeof(b2_degr));

    snprintf(
        buf, buf_len,

        "[SNAPSHOT]\r\n"
        "ENC t = %u | FL=%.2f (t: %u) | FR=%.2f (t: %u) | RL=%.2f (t: %u) | RR=%.2f (t: %u)\r\n"
        "BOARD HEALTH t = %u | Temp: %.2f (t: %u) | BATT=%.2f%% (t: %u)\r\n"
        "RX t = %u | last=%s | valid_t=%u\r\n"

        "RX PAYLOAD | x=%.2f | y=%.2f | cmd=%u | criticalB2=%s | degradedB2=%s | alive=%lu\r\n"
        "criticalB1=%s | degradedB1=%s\r\n\r\n",

        (unsigned)enc->task_last_run_ms,
        enc->wheel_speed_rpm[0], (unsigned)enc->data_last_valid_ms[0],
        enc->wheel_speed_rpm[1], (unsigned)enc->data_last_valid_ms[1],
        enc->wheel_speed_rpm[2], (unsigned)enc->data_last_valid_ms[2],
        enc->wheel_speed_rpm[3], (unsigned)enc->data_last_valid_ms[3],

        (unsigned)bh->task_last_run_ms,
        bh->temperature_degC,
        (unsigned)bh->temp_last_valid_ms,
        bh->battery_pct,
        (unsigned)bh->batt_last_valid_ms,

        (unsigned)rx->task_last_run_ms,
        CommUnpackStatusToStr(rx->last_event),
        (unsigned)rx->data_last_valid_ms,

        rx->payload.x_norm,
        rx->payload.y_norm,
        rx->payload.command,
		b2_crit,
		b2_degr,
        rx->payload.alive_counter,
		b1_crit,
		b1_degr
    );

}


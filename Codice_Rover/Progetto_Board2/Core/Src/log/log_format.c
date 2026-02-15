/**
 * @file log_format.c
 * @brief Formattazione testuale degli snapshot di diagnostica.
 */



#include <stdio.h>
#include <string.h>

#include "log/log_format.h"
#include "snapshot/supervisor_snapshot.h"

#include "shared_headers/board1_faults.h"
#include "shared_headers/board2_faults.h"

/** @brief Lunghezza buffer usata per serializzare maschere fault in stringa. */
#define LOG_FAULT_STR_LEN 64U

/**
 * @brief Converte lo stato di unpack in stringa breve.
 * @param s Stato unpack.
 * @return Stringa costante associata allo stato.
 */
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

/**
 * @brief Converte la fault mask di board 1 in stringa.
 * @param flags Maschera fault board 1.
 * @param buf Buffer di output.
 * @param len Lunghezza buffer di output.
 */
static void B1FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if (!buf || len == 0U)
    {
        return;
    }

    buf[0] = '\0';

    if (flags == FAULT_NONE)
    {
        strncpy(buf, "NONE", len);
        return;
    }
    if (flags & FAULT_TEMP) { strncat(buf, "TEMP-", len); }
    if (flags & FAULT_BATT) { strncat(buf, "BATT-", len); }
    if (flags & FAULT_RX) { strncat(buf, "RX-", len); }
    if (flags & FAULT_WHEEL_FL) { strncat(buf, "FL-", len); }
    if (flags & FAULT_WHEEL_FR) { strncat(buf, "FR-", len); }
    if (flags & FAULT_WHEEL_RL) { strncat(buf, "RL-", len); }
    if (flags & FAULT_WHEEL_RR) { strncat(buf, "RR-", len); }
    if (flags & FAULT_B2_SUP) { strncat(buf, "B2SUP-", len); }

    size_t l = strlen(buf);
    if (l > 0U) { buf[l - 1U] = '\0'; }
}

/**
 * @brief Converte la fault mask di board 2 in stringa.
 * @param flags Maschera fault board 2.
 * @param buf Buffer di output.
 * @param len Lunghezza buffer di output.
 */
static void B2FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if (!buf || len == 0U)
    {
        return;
    }

    buf[0] = '\0';

    if (flags == FAULT_NONE)
    {
        strncpy(buf, "NONE", len);
        return;
    }

    if (flags & FAULT_BLE) { strncat(buf, "BLE-", len); }
    if (flags & FAULT_IMU) { strncat(buf, "IMU-", len); }
    if (flags & FAULT_RX) { strncat(buf, "RX-", len); }
    if (flags & FAULT_B1_SUP) { strncat(buf, "B1SUP-", len); }

    size_t l = strlen(buf);
    if (l > 0U) { buf[l - 1U] = '\0'; }
}

/**
 * @brief Costruisce una stringa di log completa a partire dagli snapshot.
 * @param buf Buffer di output.
 * @param buf_len Lunghezza buffer di output.
 * @param ble Snapshot BLE.
 * @param imu Snapshot IMU.
 * @param sonar Snapshot sonar.
 * @param rx Snapshot RX.
 */
void Log_FormatSnapshot(char *buf,
                        unsigned buf_len,
                        const BleControllerSnapshot_t *ble,
                        const IMUSnapshot_t *imu,
                        const SonarSnapshot_t *sonar,
                        const RxSnapshot_t *rx)
{
    if (!buf || !ble || !imu || !sonar || !rx)
        return;

    char b1_crit[LOG_FAULT_STR_LEN];
    char b1_degr[LOG_FAULT_STR_LEN];
    char b2_crit[LOG_FAULT_STR_LEN];
    char b2_degr[LOG_FAULT_STR_LEN];

    SupervisorSnapshot_t sup;
    SupervisorSnapshot_Read(&sup);

    B1FaultFlagsToStr(rx->payload.critical_mask, b1_crit, sizeof(b1_crit));
    B1FaultFlagsToStr(rx->payload.degraded_mask, b1_degr, sizeof(b1_degr));
    B2FaultFlagsToStr(sup.critical_mask, b2_crit, sizeof(b2_crit));
    B2FaultFlagsToStr(sup.degraded_mask, b2_degr, sizeof(b2_degr));

    /* Nota MISRA: snprintf è usata solo per logging/debug. In release safety-critical puo' essere sostituito/rimosso. */
    int fmt_status = snprintf(buf, buf_len,
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

        /* IMU */
        imu->task_last_run_ms,
        imu->ax_g, imu->ay_g, imu->az_g,
        imu->gx_dps, imu->gy_dps, imu->gz_dps, imu->yaw,
        imu->temperature_degC,
        imu->data_last_valid_ms,

        /* Sonar */
        sonar->task_last_run_ms,
        sonar->dist_cm[0], sonar->data_last_valid_ms[0],
        sonar->dist_cm[1], sonar->data_last_valid_ms[1],
        sonar->dist_cm[2], sonar->data_last_valid_ms[2],

        /* Rx snapshot di board 1 */
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

        /* Stato safety locale di board 2*/
        b2_crit,
        b2_degr
    );
    if (fmt_status < 0)
    {
        buf[0] = '\0';
    }

}


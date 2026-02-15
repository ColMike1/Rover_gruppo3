/**
 * @file log_format.c
 * @brief Utility per la formattazione delle stringhe di log.
 * @details Converte flag binari e stati di sistema in stringhe leggibili per il debug tramite UART.
 * @date Jan 19, 2026
 * @author Sterm
 */

#include <stdio.h>
#include <string.h>
#include "log/log_format.h"
#include "snapshot/supervisor_snapshot.h"
#include "shared_headers/board1_faults.h"
#include "shared_headers/board2_faults.h"

/**
 * @brief Converte l'enum CommUnpackStatus_t in stringa.
 * @param s Stato dell'unpacking.
 * @return const char* Stringa descrittiva breve.
 */
static const char *CommUnpackStatusToStr(CommUnpackStatus_t s)
{
    const char *str;
    switch (s)
    {
        case COMM_UNPACK_OK:   str = "OK";   break;
        case COMM_UNPACK_NULL: str = "NULL"; break;
        case COMM_UNPACK_LEN:  str = "LEN";  break;
        case COMM_UNPACK_CRC:  str = "CRC";  break;
        default:               str = "UNK";  break;
    }
    return str;
}

/**
 * @brief Decodifica i flag di errore della Board 1 in una stringa concatenata.
 * @param flags Maschera dei bit di errore.
 * @param buf Buffer di destinazione.
 * @param len Lunghezza del buffer.
 */
void B1FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if ((buf != NULL) && (len > 0U))
    {
        buf[0] = '\0';

        if (flags == FAULT_NONE)
        {
            (void)strncpy(buf, "NONE", len - 1U);
            buf[len - 1U] = '\0';
        }
        else
        {
            if ((flags & FAULT_TEMP) != 0U)     { (void)strncat(buf, "TEMP-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_BATT) != 0U)     { (void)strncat(buf, "BATT-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_RX) != 0U)       { (void)strncat(buf, "RX-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_WHEEL_FL) != 0U) { (void)strncat(buf, "FL-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_WHEEL_FR) != 0U) { (void)strncat(buf, "FR-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_WHEEL_RL) != 0U) { (void)strncat(buf, "RL-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_WHEEL_RR) != 0U) { (void)strncat(buf, "RR-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_B2_SUP) != 0U)   { (void)strncat(buf, "B2SUP-", len - strlen(buf) - 1U); }

            size_t l = strlen(buf);
            if (l > 0U) { buf[l - 1U] = '\0'; } /* Rimuove l'ultimo trattino */
        }
    }
}

/**
 * @brief Decodifica i flag di errore della Board 2 in una stringa concatenata.
 */
void B2FaultFlagsToStr(uint32_t flags, char *buf, size_t len)
{
    if ((buf != NULL) && (len > 0U))
    {
        buf[0] = '\0';

        if (flags == FAULT_NONE)
        {
            (void)strncpy(buf, "NONE", len - 1U);
            buf[len - 1U] = '\0';
        }
        else
        {
            if ((flags & FAULT_BLE) != 0U)    { (void)strncat(buf, "BLE-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_IMU) != 0U)    { (void)strncat(buf, "IMU-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_RX) != 0U)     { (void)strncat(buf, "RX-", len - strlen(buf) - 1U); }
            if ((flags & FAULT_B1_SUP) != 0U) { (void)strncat(buf, "B1SUP-", len - strlen(buf) - 1U); }

            size_t l = strlen(buf);
            if (l > 0U) { buf[l - 1U] = '\0'; }
        }
    }
}

/**
 * @brief Crea un report testuale completo dello stato attuale del sistema.
 * @param[out] buf Buffer per la stringa finale.
 * @param[in] buf_len Lunghezza buffer.
 * @param[in] enc Snapshot motori/encoder.
 * @param[in] bh Snapshot salute scheda (temp/batt).
 * @param[in] rx Snapshot ricezione dati.
 */
void Log_FormatSnapshot(char *buf, unsigned int buf_len, const EncoderSnapshot_t *enc, const BoardHealthSnapshot_t *bh, const RxSnapshot_t *rx)
{
    if ((buf != NULL) && (enc != NULL) && (bh != NULL) && (rx != NULL))
    {
        char b1_crit[64]; char b1_degr[64];
        char b2_crit[64]; char b2_degr[64];
        static SupervisorSnapshot_t sup;

        SupervisorSnapshot_Read(&sup);

        B1FaultFlagsToStr(sup.critical_mask, b1_crit, sizeof(b1_crit));
        B1FaultFlagsToStr(sup.degraded_mask, b1_degr, sizeof(b1_degr));
        B2FaultFlagsToStr(rx->payload.critical_mask, b2_crit, sizeof(b2_crit));
        B2FaultFlagsToStr(rx->payload.degraded_mask, b2_degr, sizeof(b2_degr));

        (void)snprintf(buf, (size_t)buf_len,
            "[SNAPSHOT]\r\n"
            "ENC t = %u | FL=%.2f (t: %u) | FR=%.2f (t: %u) | RL=%.2f (t: %u) | RR=%.2f (t: %u)\r\n"
            "BOARD HEALTH t = %u | Temp: %.2f (t: %u) | BATT=%.2f%% (t: %u)\r\n"
            "RX t = %u | last=%s | valid_t=%u\r\n"
            "RX PAYLOAD | x=%.2f | y=%.2f | cmd=%u | critB2=%s | degrB2=%s | alive=%lu\r\n"
            "critB1=%s | degrB1=%s\r\n\r\n",
            (unsigned int)enc->task_last_run_ms,
            enc->wheel_speed_rpm[0], (unsigned int)enc->data_last_valid_ms[0],
            enc->wheel_speed_rpm[1], (unsigned int)enc->data_last_valid_ms[1],
            enc->wheel_speed_rpm[2], (unsigned int)enc->data_last_valid_ms[2],
            enc->wheel_speed_rpm[3], (unsigned int)enc->data_last_valid_ms[3],
            (unsigned int)bh->task_last_run_ms,
            bh->temperature_degC, (unsigned int)bh->temp_last_valid_ms,
            bh->battery_pct, (unsigned int)bh->batt_last_valid_ms,
            (unsigned int)rx->task_last_run_ms,
            CommUnpackStatusToStr(rx->last_event), (unsigned int)rx->data_last_valid_ms,
            rx->payload.x_norm, rx->payload.y_norm, (unsigned int)rx->payload.command,
            b2_crit, b2_degr, (unsigned long)rx->payload.alive_counter,
            b1_crit, b1_degr);
    }
}

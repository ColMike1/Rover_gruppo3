/**
 * @file comm_pack.c
 * @brief Funzioni per il packaging dei messaggi di comunicazione (Serializzazione).
 */

#include "comm/comm_pack.h"
#include "shared_headers/comm_message_structures.h"
#include "shared_headers/comm_crc.h"
#include <string.h>


/* ================= PACK FUNCTION ================= */

/**
 * @brief Costruisce il pacchetto di telemetria B1 (Trasmissione verso PC/Base).
 * @details Include dati degli encoder e stato del supervisore, aggiungendo header e CRC16-CCITT.
 * @param[out] buf Buffer di destinazione per il pacchetto serializzato.
 * @param[in] max_len Lunghezza massima del buffer fornito.
 * @param[in] enc Snapshot degli encoder da includere nel payload.
 * @param[in] sup Snapshot del supervisore da includere nel payload.
 * @return uint16_t Numero di byte scritti nel buffer (0 in caso di errore).
 */
uint16_t CommPack_BuildB1Tx(uint8_t *buf, uint16_t max_len, const EncoderSnapshot_t *enc, const SupervisorSnapshot_t *sup)
{
    static uint16_t tx_seq = 0U;
    uint16_t i = 0U;
    const uint16_t FRAME_LEN = (uint16_t)sizeof(CommFrameB1_t);
    const uint16_t HEADER_LEN = (uint16_t)sizeof(CommFrameHeader_t);
    const uint16_t PAYLOAD_LEN = (uint16_t)sizeof(CommPayloadB1_t);
    const uint16_t CRC_LEN = (uint16_t)sizeof(uint16_t);

    if ((buf == NULL) || (enc == NULL) || (sup == NULL))
    {
        return 0U;
    }

    if (max_len < FRAME_LEN)
    {
        return 0U;
    }

    CommFrameHeader_t hdr;
    hdr.payload_len  = PAYLOAD_LEN;
    tx_seq++;
    hdr.seq          = tx_seq;
    hdr.timestamp_ms = sup->task_last_run_ms;
    hdr.msg_id       = 0xAA55U;

    (void)memcpy(&buf[i], &hdr, HEADER_LEN);
    i += HEADER_LEN;

    CommPayloadB1_t pl;
    int wheel_idx;
    for (wheel_idx = 0; wheel_idx < 4; wheel_idx++)
    {
        pl.wheel_speed_rpm[wheel_idx] = enc->wheel_speed_rpm[wheel_idx];
    }
    pl.degraded_mask = sup->degraded_mask;
    pl.critical_mask = sup->critical_mask;
    pl.alive_counter = sup->alive_counter;

    (void)memcpy(&buf[i], &pl, PAYLOAD_LEN);
    i += PAYLOAD_LEN;

    uint16_t crc = crc16_ccitt(buf, i);
    (void)memcpy(&buf[i], &crc, CRC_LEN);
    i += CRC_LEN;

    return i;
}

/**
 * @file comm_unpack.c
 * @brief Deserializzazione e validazione frame ricevuti da board 1.
 */
#include <string.h>
#include "comm/comm_unpack.h"
#include "shared_headers/comm_crc.h"

/**
 * @brief Effettua l'unpack di un frame Board1 -> Board2.
 *
 * Verifica prima la coerenza dei puntatori, poi la lunghezza minima e infine
 * il CRC16-CCITT del frame completo.
 *
 * @param[in] buf Buffer ricevuto.
 * @param[in] len Lunghezza del buffer.
 * @param[out] header Header estratto dal frame.
 * @param[out] payload Payload estratto dal frame.
 * @return Stato dell'operazione di unpack.
 */
CommUnpackStatus_t CommUnpack_B2FromB1(const uint8_t *buf, uint16_t len, CommFrameHeader_t *header, CommPayloadB1_t *payload)
{
    if (!buf || !header || !payload)
    {
        return COMM_UNPACK_NULL;
    }

    const uint16_t HEADER_LEN = sizeof(CommFrameHeader_t);
    const uint16_t PAYLOAD_LEN = sizeof(CommPayloadB1_t);
    const uint16_t CRC_LEN   = sizeof(uint16_t);
    const uint16_t FRAME_LEN = sizeof(CommFrameB1_t);

    if (len < FRAME_LEN)
    {
        return COMM_UNPACK_LEN;
    }

    /* Verifica CRC. */
    uint16_t rx_crc;
    memcpy(&rx_crc, &buf[FRAME_LEN - CRC_LEN], CRC_LEN);

    uint16_t calc_crc = crc16_ccitt(buf, FRAME_LEN - CRC_LEN);

    if (rx_crc != calc_crc)
    {
        return COMM_UNPACK_CRC;
    }

    uint16_t i = 0U;

    /* Estrazione header. */
    memcpy(header, &buf[i], HEADER_LEN);
    i += HEADER_LEN;

    /* Estrazione payload. */
    memcpy(payload, &buf[i], PAYLOAD_LEN);

    return COMM_UNPACK_OK;
}


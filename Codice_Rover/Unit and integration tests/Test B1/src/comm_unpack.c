/*
 * comm_unpack.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */


#include <string.h>
#include "comm_unpack.h"
#include "comm_crc.h"

CommUnpackStatus_t CommUnpack_B1FromB2(const uint8_t *buf,
                                      uint16_t len,
                                      CommFrameHeader_t *header,
                                      CommPayloadB2_t *payload)
{
    if (!buf || !header || !payload)
        return COMM_UNPACK_NULL;

    const uint16_t HEADER_LEN = sizeof(CommFrameHeader_t);
    const uint16_t PAYLOAD_LEN = sizeof(CommPayloadB2_t);
    const uint16_t CRC_LEN   = sizeof(uint16_t);
    const uint16_t FRAME_LEN = sizeof(CommFrameB2_t);

    if (len < FRAME_LEN)
        return COMM_UNPACK_LEN;

    /* ---------- CRC check ---------- */
    uint16_t rx_crc;
    memcpy(&rx_crc, &buf[FRAME_LEN - CRC_LEN], CRC_LEN);

    uint16_t calc_crc = crc16_ccitt(buf, FRAME_LEN - CRC_LEN);

    if (rx_crc != calc_crc)
        return COMM_UNPACK_CRC;

    uint16_t i = 0;

    /* ---------- Header ---------- */
    memcpy(header, &buf[i], HEADER_LEN);
    i += HEADER_LEN;

    /* ---------- Payload ---------- */
    memcpy(payload, &buf[i], PAYLOAD_LEN);

    return COMM_UNPACK_OK;
}

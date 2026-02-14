/*
 * comm_unpack.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */


#include <string.h>
#include "comm/comm_unpack.h"
#include "shared_headers/comm_crc.h"

#define HEADER_LEN  	((uint16_t)sizeof(CommFrameHeader_t))
#define PAYLOAD_LEN  	((uint16_t)sizeof(CommPayloadB2_t))
#define CRC_LEN 		((uint16_t)sizeof(uint16_t))
#define FRAME_LEN 		((uint16_t)sizeof(CommFrameB2_t))

CommUnpackStatus_t CommUnpack_B1FromB2(const uint8_t *buf, uint16_t len, CommFrameHeader_t *header, CommPayloadB2_t *payload)
{
	CommUnpackStatus_t status = COMM_UNPACK_OK;

    if (!buf || !header || !payload){
        status = COMM_UNPACK_NULL;
    }
    else if (len < FRAME_LEN){
    	status = COMM_UNPACK_LEN;
    }
    else{
    	/* ---------- CRC check ---------- */
    	uint16_t rx_crc;
		memcpy(&rx_crc, &buf[FRAME_LEN - CRC_LEN], CRC_LEN);

		uint16_t calc_crc = crc16_ccitt(buf, FRAME_LEN - CRC_LEN);

		if (rx_crc != calc_crc){
			return COMM_UNPACK_CRC;
		}

		uint16_t i = 0;

		/* ---------- Header ---------- */
		memcpy(header, &buf[i], HEADER_LEN);
		i += HEADER_LEN;

		/* ---------- Payload ---------- */
		memcpy(payload, &buf[i], PAYLOAD_LEN);

		status = COMM_UNPACK_OK;
    }

    return status;
}

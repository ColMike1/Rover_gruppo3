/*
 * comm_unpack.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */

#ifndef INC_COMM_COMM_UNPACK_H_
#define INC_COMM_COMM_UNPACK_H_

#include <stdint.h>
#include "comm_message_structures.h"
#include "comm_status.h"

/**
 * @brief Unpack frame Board2 -> Board1
 *
 * @param buf       RX buffer
 * @param len       RX length
 * @param header    output header
 * @param payload   output payload
 * @return CommUnpackStatus_t
 */
CommUnpackStatus_t CommUnpack_B1FromB2(const uint8_t *buf,
                                      uint16_t len,
                                      CommFrameHeader_t *header,
                                      CommPayloadB2_t *payload);

#endif /* INC_COMM_COMM_UNPACK_H_ */

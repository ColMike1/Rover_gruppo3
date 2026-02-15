/**
 * @file comm_unpack.h
 * @brief API di deserializzazione frame ricevuti da board 1.
 */

#ifndef INC_COMM_COMM_UNPACK_H_
#define INC_COMM_COMM_UNPACK_H_

#include <stdint.h>

#include "shared_headers/comm_message_structures.h"
#include "shared_headers/comm_status.h"

/**
 * @brief Effettua unpack e validazione di un frame Board1 -> Board2.
 * @param buf Buffer RX.
 * @param len Lunghezza RX.
 * @param header Header di output.
 * @param payload Payload di output.
 * @return Stato dell'unpack.
 */
CommUnpackStatus_t CommUnpack_B2FromB1(const uint8_t *buf, uint16_t len, CommFrameHeader_t *header, CommPayloadB1_t *payload);

#endif /* INC_COMM_COMM_UNPACK_H_ */

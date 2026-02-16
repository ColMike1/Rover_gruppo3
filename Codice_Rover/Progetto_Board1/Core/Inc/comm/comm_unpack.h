/**
 * @file comm_unpack.h
 * @brief Funzioni per il parsing e l'estrazione dei dati dai frame ricevuti.
 */

#ifndef INC_COMM_COMM_UNPACK_H_
#define INC_COMM_COMM_UNPACK_H_

#include <stdint.h>
#include "shared_headers/comm_message_structures.h"
#include "shared_headers/comm_status.h"

/**
 * @brief Decodifica un frame ricevuto da Board2 destinato a Board1.
 * @param[in]  buf       Puntatore al buffer RX contenente il frame grezzo.
 * @param[in]  len       Lunghezza dei dati nel buffer.
 * @param[out] header    Puntatore alla struttura dove estrarre l'header del frame.
 * @param[out] payload   Puntatore alla struttura dove estrarre il carico utile (payload).
 * @return CommUnpackStatus_t Stato della decodifica (es. OK, errore CRC, lunghezza errata).
 */
CommUnpackStatus_t CommUnpack_B1FromB2(const uint8_t *buf, uint16_t len,  CommFrameHeader_t *header, CommPayloadB2_t *payload);

#endif /* INC_COMM_COMM_UNPACK_H_ */

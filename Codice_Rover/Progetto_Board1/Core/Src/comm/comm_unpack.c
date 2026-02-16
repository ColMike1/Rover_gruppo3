/**
 * @file comm_unpack.c
 * @brief Funzioni per l'unpackaging e la validazione dei messaggi ricevuti (Deserializzazione).
 * @date Jan 11, 2026
 * @author Sterm
 */

#include <string.h>
#include "comm/comm_unpack.h"
#include "shared_headers/comm_crc.h"

/** @brief Lunghezza dell'header comune. */
#define HEADER_LEN  	((uint16_t)sizeof(CommFrameHeader_t))
/** @brief Lunghezza specifica del payload B2. */
#define PAYLOAD_LEN  	((uint16_t)sizeof(CommPayloadB2_t))
/** @brief Lunghezza del campo CRC16. */
#define CRC_LEN 		((uint16_t)sizeof(uint16_t))
/** @brief Lunghezza totale attesa per il frame B2. */
#define FRAME_LEN 		((uint16_t)sizeof(CommFrameB2_t))

/**
 * @brief Valida il pacchetto ricevuto tramite CRC ed estrae header e payload.
 * @param[in] buf Buffer contenente il pacchetto grezzo ricevuto.
 * @param[in] len Lunghezza del buffer fornito.
 * @param[out] header Puntatore alla struttura header da popolare.
 * @param[out] payload Puntatore alla struttura payload B2 da popolare.
 * @return CommUnpackStatus_t Stato dell'operazione (OK, errore CRC, errore lunghezza, etc).
 */
CommUnpackStatus_t CommUnpack_B1FromB2(const uint8_t *buf, uint16_t len, CommFrameHeader_t *header, CommPayloadB2_t *payload)
{
    CommUnpackStatus_t status = COMM_UNPACK_OK;

    if ((buf == NULL) || (header == NULL) || (payload == NULL))
    {
        status = COMM_UNPACK_NULL;
    }
    else if (len < FRAME_LEN)
    {
        status = COMM_UNPACK_LEN;
    }
    else
    {
        uint16_t rx_crc = 0U;
        (void)memcpy(&rx_crc, &buf[FRAME_LEN - CRC_LEN], CRC_LEN);

        uint16_t calc_crc = crc16_ccitt(buf, FRAME_LEN - CRC_LEN);

        if (rx_crc != calc_crc)
        {
            status = COMM_UNPACK_CRC;
        }
        else
        {
            uint16_t i = 0U;
            (void)memcpy(header, &buf[i], HEADER_LEN);
            i += HEADER_LEN;
            (void)memcpy(payload, &buf[i], PAYLOAD_LEN);
        }
    }
    return status;
}

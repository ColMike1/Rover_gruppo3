/**
 * @file comm_pack.h
 * @brief Funzioni per il pacchettizzamento dei dati di telemetria e stato.
 */

#ifndef INC_COMM_COMM_PACK_H_
#define INC_COMM_COMM_PACK_H_

#include <stdint.h>
#include "snapshot/supervisor_snapshot.h"
#include "snapshot/encoder_snapshot.h"

/**
 * @brief Costruisce il frame di stato per la comunicazione da Board1 a Board2.
 * @param[out] buf Buffer di uscita dove verrà scritto il frame.
 * @param[in]  max_len Dimensione massima del buffer.
 * @param[in]  enc Puntatore allo snapshot degli encoder da includere.
 * @param[in]  sup Puntatore allo snapshot del supervisore da includere.
 * @return Lunghezza del frame in byte (0 in caso di errore).
 */
uint16_t CommPack_BuildB1Tx(uint8_t *buf,
                                uint16_t max_len,
                                const EncoderSnapshot_t *enc,
                                const SupervisorSnapshot_t *sup);

#endif /* INC_COMM_COMM_PACK_H_ */

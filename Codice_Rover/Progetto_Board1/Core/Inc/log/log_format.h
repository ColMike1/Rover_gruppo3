/**
 * @file log_format.h
 * @brief Utility per la formattazione delle stringhe di log.
 */

#ifndef INC_LOG_LOG_FORMAT_H_
#define INC_LOG_LOG_FORMAT_H_

#include "snapshot/encoder_snapshot.h"
#include "snapshot/board_health_snapshot.h"
#include "snapshot/rx_snapshot.h"

/**
 * @brief Formatta i dati degli snapshot in una stringa leggibile per il logging.
 * @param[out] buf Buffer di destinazione per la stringa formattata.
 * @param[in]  buf_len Dimensione del buffer.
 * @param[in]  enc Puntatore allo snapshot degli encoder.
 * @param[in]  bh  Puntatore allo snapshot board health.
 * @param[in]  rx  Puntatore allo snapshot di ricezione.
 */
void Log_FormatSnapshot(char *buf,
                        unsigned buf_len,
                        const EncoderSnapshot_t *enc,
                        const BoardHealthSnapshot_t *bh,
						const RxSnapshot_t *rx);

#endif /* INC_LOG_LOG_FORMAT_H_ */

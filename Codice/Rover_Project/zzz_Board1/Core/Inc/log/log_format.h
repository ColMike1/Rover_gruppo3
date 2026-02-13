/*
 * log_format.h
 *
 *  Created on: Jan 19, 2026
 *      Author: Sterm
 */

#ifndef INC_LOG_LOG_FORMAT_H_
#define INC_LOG_LOG_FORMAT_H_

#include "snapshot/encoder_snapshot.h"
#include "snapshot/board_health_snapshot.h"
#include "snapshot/rx_snapshot.h"


void Log_FormatSnapshot(char *buf,
                        unsigned buf_len,
                        const EncoderSnapshot_t *enc,
                        const BoardHealthSnapshot_t *bh,
						const RxSnapshot_t *rx);


#endif /* INC_LOG_LOG_FORMAT_H_ */

/*
 * comm_pack.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */

#ifndef INC_COMM_COMM_PACK_H_
#define INC_COMM_COMM_PACK_H_


#include <stdint.h>

#include "supervisor_snapshot.h"
#include "encoder_snapshot.h"


/**
 * @brief Build Board1 -> Board2 status frame
 * @param[out] buf Output buffer
 * @param[in]  max_len Buffer size
 * @param[in]  enc Encoder snapshot
 * @param[in]  sup Supervision snapshot
 * @return Frame length in bytes (0 on error)
 */
uint16_t CommPack_BuildB1Tx(uint8_t *buf,
                                uint16_t max_len,
                                const EncoderSnapshot_t *enc,
                                const SupervisorSnapshot_t *sup);

#endif /* INC_COMM_COMM_PACK_H_ */

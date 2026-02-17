/*
 * rx_snapshot.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef SRC_SNAPSHOT_RX_SNAPSHOT_H_
#define SRC_SNAPSHOT_RX_SNAPSHOT_H_

#include <stdint.h>
#include "comm_status.h"
#include "comm_message_structures.h"
#include "cmsis_os2.h"
/* ===== Snapshot ===== */
typedef struct
{
    /* Payload ricevuto (raw, applicativo) */
    CommPayloadB1_t payload;

    /* Status dell'ultimo evento RX osservato */
    CommUnpackStatus_t last_event;

    uint32_t task_last_run_ms;     /* ultima esecuzione del task */
    uint32_t data_last_valid_ms;  /* ultimo istante in cui i dati sono validi */

} RxSnapshot_t;

void RxSnapshot_MutexInit(osMutexId_t mutex_handle);

/* ===== Writer (RX task only) ===== */
void RxSnapshot_Write(RxSnapshot_t *src);

/* ===== Reader (Supervisor / others) ===== */
void RxSnapshot_Read(RxSnapshot_t *dst);

#endif /* SRC_SNAPSHOT_RX_SNAPSHOT_H_ */

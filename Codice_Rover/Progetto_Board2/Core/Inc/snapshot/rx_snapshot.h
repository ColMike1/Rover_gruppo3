/**
 * @file rx_snapshot.h
 * @brief Definizione dello snapshot RX e API thread-safe.
 */

#ifndef INC_SNAPSHOT_RX_SNAPSHOT_H_
#define INC_SNAPSHOT_RX_SNAPSHOT_H_

#include <stdint.h>
#include "shared_headers/comm_status.h"
#include "shared_headers/comm_message_structures.h"
#include "cmsis_os.h"
/** @brief Snapshot dei dati ricevuti dal canale di comunicazione. */
typedef struct
{
    /* Payload ricevuto */
    CommPayloadB1_t payload;

    /* Status dell'ultimo evento RX osservato */
    CommUnpackStatus_t last_event;

    uint32_t task_last_run_ms;     /* ultima esecuzione del task */
    uint32_t data_last_valid_ms;  /* ultimo istante in cui i dati sono validi */

} RxSnapshot_t;

/**
 * @brief Registra il mutex usato per proteggere lo snapshot.
 * @param mutex_handle Handle del mutex creato all'esterno.
 */
void RxSnapshot_MutexInit(osMutexId_t mutex_handle);

/**
 * @brief Scrive lo snapshot RX in sezione critica.
 * @param src Puntatore al dato sorgente da copiare.
 */
void RxSnapshot_Write(const RxSnapshot_t *src);

/**
 * @brief Legge lo snapshot RX in sezione critica.
 * @param dst Puntatore al buffer di destinazione.
 */
void RxSnapshot_Read(RxSnapshot_t *dst);

#endif /* INC_SNAPSHOT_RX_SNAPSHOT_H_ */

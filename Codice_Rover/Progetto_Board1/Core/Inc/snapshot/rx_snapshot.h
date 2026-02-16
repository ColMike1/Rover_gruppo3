/**
 * @file rx_snapshot.h
 * @brief Buffer protetto per i dati ricevuti dall'altra board.
 */

#ifndef INC_SNAPSHOT_RX_SNAPSHOT_H_
#define INC_SNAPSHOT_RX_SNAPSHOT_H_

#include <stdint.h>
#ifndef MATLAB_MEX_FILE
  #include "cmsis_os.h"
#endif
#include "shared_headers/comm_message_structures.h"
#include "shared_headers/comm_status.h"
#include "log/wcet_monitor.h"
#include "main.h"

/**
 * @struct RxSnapshot_t
 * @brief Snapshot dei dati ricevuti via comunicazione.
 */
typedef struct
{
    CommPayloadB2_t payload;      /**< Carico utile dell'ultimo messaggio valido ricevuto */
    CommUnpackStatus_t last_event; /**< Esito dell'ultimo tentativo di unpacking */

    uint32_t task_last_run_ms;    /**< Timestamp dell'ultimo tentativo di ricezione */
    uint32_t data_last_valid_ms;  /**< Timestamp dell'ultimo dato valido ricevuto */
} RxSnapshot_t;

#ifndef MATLAB_MEX_FILE
/**
 * @brief Inizializza il mutex per l'accesso thread-safe allo snapshot RX.
 */
void RxSnapshot_MutexInit(osMutexId_t mutex_handle);
#endif

/**
 * @brief Aggiorna lo snapshot con i nuovi dati ricevuti (solo task RX).
 */
void RxSnapshot_Write(const RxSnapshot_t *src);

/**
 * @brief Recupera i dati ricevuti per l'elaborazione (es. da parte del Supervisore).
 */
void RxSnapshot_Read(RxSnapshot_t *dst);

#endif /* INC_SNAPSHOT_RX_SNAPSHOT_H_ */

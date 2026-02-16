/**
 * @file supervisor_snapshot.h
 * @brief Definizione dello snapshot supervisore e API thread-safe.
 */

#ifndef INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_
#define INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_

#include <stdint.h>
#include <stdbool.h>
#include "shared_headers/supervisor_command.h"
#include "cmsis_os.h"

/** @brief Snapshot dello stato prodotto dal supervisore Board 2. */
typedef struct
{
	/* Timestamp decisione */
	uint32_t task_last_run_ms;

  /* Fault di board 2 presenti derivanti da stato degradato delle periferiche. */
  uint32_t degraded_mask;

  /* Fault di board 2 presenti derivanti da stato critico delle periferiche. */
  uint32_t critical_mask;

  /* Comando deciso da board 2 */
  SupervisorCommand_t command;

  /* Coerenza dei dati ricevuti da imu, utile a rilevare motor fault */
  bool isMotionConsistent;

  /* Heartbeat / alive counter */
  uint32_t alive_counter;

} SupervisorSnapshot_t;


/**
 * @brief Registra il mutex usato per proteggere lo snapshot.
 * @param mutex_handle Handle del mutex creato all'esterno.
 */
void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle);

/**
 * @brief Scrive lo snapshot supervisore in sezione critica.
 * @param src Puntatore al dato sorgente da copiare.
 */
void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src);

/**
 * @brief Legge lo snapshot supervisore in sezione critica.
 * @param dst Puntatore al buffer di destinazione.
 */
void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst);


#endif /* INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_ */

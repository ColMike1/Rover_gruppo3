/*
 * supervisor_snapshot.h
 *
 *  Created on: Jan 16, 2026
 *      Author: Sterm
 */

#ifndef INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_
#define INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_

#include <stdint.h>
#include <stdbool.h>
#include "shared_headers/supervisor_command.h"
#include "cmsis_os.h"



/* ===== Snapshot Supervisore Board 2 ===== */
typedef struct
{
	/* Timestamp decisione */
	uint32_t task_last_run_ms;

  /* Fault di board 2 presenti che comportano uno stato degradato*/
  uint32_t degraded_mask;

  /* Fault di board 2 presenti che comportano uno stato di emergenza*/
  uint32_t critical_mask;

  /* Comando semantico deciso da board 2 */
  SupervisorCommand_t command;

  /* Coerenza dei dati ricevuti da imu, utile a rilevare motor fault */
  bool isMotionConsistent;

  /* Heartbeat / alive counter */
  uint32_t alive_counter;

} SupervisorSnapshot_t;



/* ================= API ================= */
void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle);

/* Writer: solo task Supervisore B2 */
void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src);

/* Reader: solo task TX (anche controllo e attuazione credo)*/
void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst);


#endif /* INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_ */

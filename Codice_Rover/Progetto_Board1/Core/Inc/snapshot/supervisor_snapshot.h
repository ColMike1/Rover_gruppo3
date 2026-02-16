/**
 * @file supervisor_snapshot.h
 * @brief Buffer protetto per le decisioni prese dal Supervisore di Board 1.
 */

#ifndef INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_
#define INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_

#include <stdint.h>
#include <stdbool.h>
#include "shared_headers/board1_faults.h"
#include "shared_headers/supervisor_command.h"
#ifndef MATLAB_MEX_FILE
  #include "cmsis_os.h"
#endif

/**
 * @struct SupervisorSnapshot_t
 * @brief Snapshot delle decisioni, dei fault e dei comandi autorizzati.
 */
typedef struct
{
  uint32_t task_last_run_ms;   /**< Timestamp dell'ultima decisione presa [ms] */
  uint32_t degraded_mask;      /**< Maschera dei fault che causano stato degradato */
  uint32_t critical_mask;      /**< Maschera dei fault che causano stato di emergenza */

  float speed_ref_rpm;         /**< Velocità longitudinale autorizzata [RPM] */
  float steering_cmd;          /**< Comando sterzata autorizzato */

  SupervisorCommand_t current_action; /**< Azione corrente decisa dal supervisore */
  bool isBoardActuating;              /**< Stato di attivazione delle uscite di potenza */

  uint8_t alive_counter;              /**< Counter incrementale per heartbeat */
} SupervisorSnapshot_t;

#ifndef MATLAB_MEX_FILE
/**
 * @brief Inizializza il mutex per l'accesso thread-safe allo snapshot supervisore.
 */
void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle);
#endif

/**
 * @brief Scrive le decisioni del supervisore (solo task Supervisione).
 */
void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src);

/**
 * @brief Legge le decisioni del supervisore (usato da Controllo, Attuazione, TX).
 */
void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst);

#endif /* INC_SNAPSHOT_SUPERVISOR_SNAPSHOT_H_ */

/**
 * @file encoder_snapshot.h
 * @brief Buffer protetto per i dati letti dagli encoder.
 */

#ifndef INC_SNAPSHOT_CONTROL_CYCLE_SNAPSHOT_H_
#define INC_SNAPSHOT_CONTROL_CYCLE_SNAPSHOT_H_

#include <stdint.h>
#include <stdbool.h>
#ifndef MATLAB_MEX_FILE
  #include "cmsis_os2.h"
#endif

/**
 * @struct EncoderSnapshot_t
 * @brief Snapshot delle velocità ruota e dello stato feedback.
 */
typedef struct
{
  float wheel_speed_rpm[4];    /**< Velocità misurate per le 4 ruote [RPM] */
  bool hasNoFeedback[4];	   /**< Vero se il motore non risponde al comando (0 RPM con comando attivo) */

  uint32_t task_last_run_ms;      /**< Timestamp dell'ultima esecuzione del task [ms] */
  uint32_t data_last_valid_ms[4]; /**< Timestamp di validità per singolo encoder [ms] */
} EncoderSnapshot_t;

#ifndef MATLAB_MEX_FILE
/**
 * @brief Inizializza il mutex per l'accesso thread-safe allo snapshot encoder.
 */
void EncoderSnapshot_MutexInit(osMutexId_t mutex_handle);
#endif

/**
 * @brief Scrive i dati degli encoder nello snapshot.
 */
void EncoderSnapshot_Write(EncoderSnapshot_t *src);

/**
 * @brief Legge l'ultimo snapshot encoder disponibile.
 */
void EncoderSnapshot_Read(EncoderSnapshot_t *dst);

#endif /* INC_SNAPSHOT_CONTROL_CYCLE_SNAPSHOT_H_ */

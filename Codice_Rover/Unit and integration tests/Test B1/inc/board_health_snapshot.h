/**
 * @file board_health_snapshot.h
 * @brief Buffer protetto per lo scambio dati dello stato hardware.
 */

#ifndef INC_SNAPSHOT_BOARD_HEALTH_SNAPSHOT_H_
#define INC_SNAPSHOT_BOARD_HEALTH_SNAPSHOT_H_

#include <stdint.h>
#ifndef MATLAB_MEX_FILE
  #include "cmsis_os2.h"
#endif

/**
 * @struct BoardHealthSnapshot_t
 * @brief Snapshot dei parametri vitali della scheda.
 */
typedef struct
{
  float temperature_degC;		/**< Ultima lettura valida della temperatura */
  float battery_pct;			/**< Ultima lettura valida della batteria in percentuale */

  uint32_t task_last_run_ms;     /**< Timestamp dell'ultima esecuzione del task [ms] */

  uint32_t temp_last_valid_ms;	/**< Timestamp dell'ultima rilevazione temperatura valida */
  uint32_t batt_last_valid_ms;	/**< Timestamp dell'ultima rilevazione batteria valida */
} BoardHealthSnapshot_t;

#ifndef MATLAB_MEX_FILE
/**
 * @brief Inizializza il mutex per l'accesso thread-safe allo snapshot.
 * @param mutex_handle Handle del mutex FreeRTOS/CMSIS.
 */
void BoardHealthSnapshot_MutexInit(osMutexId_t mutex_handle);
#endif

/**
 * @brief Scrive i nuovi dati nello snapshot (API per il produttore).
 * @param src Puntatore ai dati da copiare.
 */
void BoardHealthSnapshot_Write(BoardHealthSnapshot_t *src);

/**
 * @brief Legge i dati correnti dallo snapshot (API per i consumatori).
 * @param dst Puntatore dove copiare i dati letti.
 */
void BoardHealthSnapshot_Read(BoardHealthSnapshot_t *dst);

#endif /* INC_SNAPSHOT_BOARD_HEALTH_SNAPSHOT_H_ */

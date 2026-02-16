/**
 * @file sonar_snapshot.h
 * @brief Definizione dello snapshot sonar e API thread-safe.
 */

#ifndef INC_SNAPSHOT_SONAR_SNAPSHOT_H_
#define INC_SNAPSHOT_SONAR_SNAPSHOT_H_

#include <stdint.h>
#include "cmsis_os.h"

/** @brief Snapshot delle distanze sonar e dati temporali. */
typedef struct
{
  uint16_t dist_cm[3];


  uint32_t task_last_run_ms;     /* ultima esecuzione del task */
  uint32_t data_last_valid_ms[3];  /* ultimo istante in cui i dati sono validi */
} SonarSnapshot_t;

/**
 * @brief Registra il mutex usato per proteggere lo snapshot.
 * @param mutex_handle Handle del mutex creato all'esterno.
 */
void SonarSnapshot_MutexInit(osMutexId_t mutex_handle);

/**
 * @brief Scrive lo snapshot sonar in sezione critica.
 * @param src Puntatore al dato sorgente da copiare.
 */
void SonarSnapshot_Write(const SonarSnapshot_t *src);

/**
 * @brief Legge lo snapshot sonar in sezione critica.
 * @param dst Puntatore al buffer di destinazione.
 */
void SonarSnapshot_Read(SonarSnapshot_t *dst);




#endif /* INC_SNAPSHOT_SONAR_SNAPSHOT_H_ */

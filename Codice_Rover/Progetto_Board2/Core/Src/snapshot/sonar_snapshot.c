/**
 * @file sonar_snapshot.c
 * @brief Storage thread-safe dello snapshot sonar.
 */


#include "snapshot/sonar_snapshot.h"

/** @brief Copia condivisa dello snapshot sonar. */
static SonarSnapshot_t snapshot;

/** @brief Mutex associato allo snapshot sonar. */
static osMutexId_t snapshot_mutex;

/**
 * @brief Registra il mutex da usare per l'accesso allo snapshot.
 * @param mutex_handle Handle mutex creato all'esterno.
 */
void SonarSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive lo snapshot sonar in sezione critica.
 * @param src Dato sorgente da copiare.
 */
void SonarSnapshot_Write(const SonarSnapshot_t *src)
{
    if (src == NULL)
    {
        return;
    }

    (void)osMutexAcquire(snapshot_mutex, osWaitForever);
    snapshot = *src;
    (void)osMutexRelease(snapshot_mutex);
}

/**
 * @brief Legge lo snapshot sonar in sezione critica.
 * @param dst Destinazione in cui copiare il dato.
 */
void SonarSnapshot_Read(SonarSnapshot_t *dst)
{
    if (dst == NULL)
    {
        return;
    }

    (void)osMutexAcquire(snapshot_mutex, osWaitForever);
    *dst = snapshot;
    (void)osMutexRelease(snapshot_mutex);
}

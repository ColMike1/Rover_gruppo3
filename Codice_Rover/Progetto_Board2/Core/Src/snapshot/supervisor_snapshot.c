/**
 * @file supervisor_snapshot.c
 * @brief Storage thread-safe dello snapshot supervisore.
 */


#include "snapshot/supervisor_snapshot.h"

/** @brief Copia condivisa dello snapshot supervisore. */
static SupervisorSnapshot_t snapshot;

/** @brief Mutex associato allo snapshot supervisore. */
static osMutexId_t snapshot_mutex;

/**
 * @brief Registra il mutex da usare per l'accesso allo snapshot.
 * @param mutex_handle Handle mutex creato all'esterno.
 */
void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive lo snapshot supervisore in sezione critica.
 * @param src Dato sorgente da copiare.
 */
void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src)
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
 * @brief Legge lo snapshot supervisore in sezione critica.
 * @param dst Destinazione in cui copiare il dato.
 */
void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst)
{
    if (dst == NULL)
    {
        return;
    }

    (void)osMutexAcquire(snapshot_mutex, osWaitForever);
    *dst = snapshot;
    (void)osMutexRelease(snapshot_mutex);
}

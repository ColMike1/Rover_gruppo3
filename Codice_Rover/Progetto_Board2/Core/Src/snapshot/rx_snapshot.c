/**
 * @file rx_snapshot.c
 * @brief Storage thread-safe dello snapshot RX.
 */


#include "snapshot/rx_snapshot.h"

/** @brief Copia condivisa dello snapshot RX. */
static RxSnapshot_t snapshot;

/** @brief Mutex associato allo snapshot RX. */
static osMutexId_t snapshot_mutex;

/**
 * @brief Registra il mutex da usare per l'accesso allo snapshot.
 * @param mutex_handle Handle mutex creato all'esterno.
 */
void RxSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive lo snapshot RX in sezione critica.
 * @param src Dato sorgente da copiare.
 */
void RxSnapshot_Write(const RxSnapshot_t *src)
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
 * @brief Legge lo snapshot RX in sezione critica.
 * @param dst Destinazione in cui copiare il dato.
 */
void RxSnapshot_Read(RxSnapshot_t *dst)
{
    if (dst == NULL)
    {
        return;
    }

    (void)osMutexAcquire(snapshot_mutex, osWaitForever);
    *dst = snapshot;
    (void)osMutexRelease(snapshot_mutex);
}

/**
 * @file rx_snapshot.c
 * @brief Gestione thread-safe del payload ricevuto dalla comunicazione (UART/Radio).
 * @details Mantiene lo stato dell'ultima ricezione valida e dei relativi flag di errore
 * della Board 2.
 */

#include "snapshot/rx_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

static RxSnapshot_t snapshot;
static osMutexId_t snapshot_mutex = NULL;

void RxSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

void RxSnapshot_Write(const RxSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

void RxSnapshot_Read(RxSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

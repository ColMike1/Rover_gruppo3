/**
 * @file supervisor_snapshot.c
 * @brief Container per lo stato del supervisore di sistema.
 * @details Include maschere di errore (critical/degraded) e azioni correnti.
 * È fondamentale per coordinare la logica di fallback e le protezioni.
 */

#include "snapshot/supervisor_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

static SupervisorSnapshot_t snapshot;
static osMutexId_t snapshot_mutex = NULL;

void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

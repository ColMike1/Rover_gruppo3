/**
 * @file encoder_snapshot.c
 * @brief Container sicuro per i dati di velocità e feedback degli encoder.
 * @details Permette al task Encoder di pubblicare i dati e al task Controllo di
 * consumarli senza race conditions.
 */

#include "snapshot/encoder_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

static EncoderSnapshot_t snapshot;
static osMutexId_t snapshot_mutex = NULL;

void EncoderSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

void EncoderSnapshot_Write(const EncoderSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

void EncoderSnapshot_Read(EncoderSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

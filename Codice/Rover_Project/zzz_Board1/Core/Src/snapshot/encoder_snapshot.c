/*
 * encoder_snapshot.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */


#include "snapshot/encoder_snapshot.h"

#include "log/wcet_monitor.h"
#include "main.h"

/* Snapshot storage */
static EncoderSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;


void EncoderSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}

/* ===== Writer API ===== */
void EncoderSnapshot_Write(const EncoderSnapshot_t *src)
{
    if (src == NULL)
        return;


    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;    /* struct copy */
    osMutexRelease(snapshot_mutex);

}

/* ===== Reader API ===== */
void EncoderSnapshot_Read(EncoderSnapshot_t *dst)
{
    if (dst == NULL)
        return;


    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;    /* struct copy */
    osMutexRelease(snapshot_mutex);

}

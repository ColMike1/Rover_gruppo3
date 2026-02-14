/*
 * rx_snapshot.c
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#include "snapshot/rx_snapshot.h"

#include "log/wcet_monitor.h"
#include "main.h"


/* Snapshot storage */
static RxSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;


void RxSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}

/* ===== Writer ===== */
void RxSnapshot_Write(const RxSnapshot_t *src)
{
    if (src == NULL)
        return;


    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;    /* struct copy */
    osMutexRelease(snapshot_mutex);

}

/* ===== Reader ===== */
void RxSnapshot_Read(RxSnapshot_t *dst)
{
    if (dst == NULL)
        return;


    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;    /* struct copy */
    osMutexRelease(snapshot_mutex);

}

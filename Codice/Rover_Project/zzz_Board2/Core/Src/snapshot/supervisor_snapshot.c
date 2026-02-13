/*
 * supervisor_snapshot.c
 *
 *  Created on: Jan 16, 2026
 *      Author: Sterm
 */


#include "snapshot/supervisor_snapshot.h"




/* Snapshot storage */
static SupervisorSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;


void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}

/* ================= API ================= */

void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src)
{
    if (src == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;
    osMutexRelease(snapshot_mutex);
}

void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst)
{
    if (dst == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;
    osMutexRelease(snapshot_mutex);
}

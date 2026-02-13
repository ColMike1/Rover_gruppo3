/*
 * sonar_snapshot.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */


#include "snapshot/sonar_snapshot.h"




/* Snapshot storage */
static SonarSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;


void SonarSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}

void SonarSnapshot_Write(const SonarSnapshot_t *src)
{
    if (src == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;
    osMutexRelease(snapshot_mutex);
}

void SonarSnapshot_Read(SonarSnapshot_t *dst)
{
    if (dst == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;
    osMutexRelease(snapshot_mutex);
}

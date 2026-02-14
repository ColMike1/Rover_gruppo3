/*
 * imu_snapshot.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */


#include "snapshot/imu_snapshot.h"

/* Snapshot storage */
static IMUSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;

void IMUSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}



/* ================= API ================= */

void IMUSnapshot_Write(const IMUSnapshot_t *src)
{
    if (src == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;
    osMutexRelease(snapshot_mutex);
}

void IMUSnapshot_Read(IMUSnapshot_t *dst)
{
    if (dst == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;
    osMutexRelease(snapshot_mutex);
}

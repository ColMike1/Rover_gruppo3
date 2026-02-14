/*
 * ble_controller_snapshot.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Sterm
 */


#include "snapshot/ble_controller_snapshot.h"

/* Snapshot storage */
static BleControllerSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;


void BleControllerSnapshot_MutexInit(osMutexId_t mutex_handle)
{
	if (snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}


void BleControllerSnapshot_Write(const BleControllerSnapshot_t *src)
{
    if (src == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;
    osMutexRelease(snapshot_mutex);
}

void BleControllerSnapshot_Read(BleControllerSnapshot_t *dst)
{
    if (dst == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;
    osMutexRelease(snapshot_mutex);
}

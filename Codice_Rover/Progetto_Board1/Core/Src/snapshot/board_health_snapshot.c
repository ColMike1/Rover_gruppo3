/*
 * board_health_snapshot.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */

#include "log/wcet_monitor.h"
#include "main.h"
#include "snapshot/board_health_snapshot.h"


/* Istanza dello snapshot */
static BoardHealthSnapshot_t snapshot;

/* Mutex */
static osMutexId_t snapshot_mutex;

void BoardHealthSnapshot_MutexInit(osMutexId_t mutex_handle){
	if(snapshot_mutex == NULL){
		snapshot_mutex = mutex_handle;
	}
}

void BoardHealthSnapshot_Write(const BoardHealthSnapshot_t *src){
    if (src == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    snapshot = *src;    /* copia atomica della struttura */
    osMutexRelease(snapshot_mutex);
}

void BoardHealthSnapshot_Read(BoardHealthSnapshot_t *dst){
    if (dst == NULL)
        return;

    osMutexAcquire(snapshot_mutex,osWaitForever);
    *dst = snapshot;    /* copia atomica della struttura */
    osMutexRelease(snapshot_mutex);
}

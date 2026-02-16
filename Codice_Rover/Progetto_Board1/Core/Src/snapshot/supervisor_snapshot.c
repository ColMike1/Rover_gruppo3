/**
 * @file supervisor_snapshot.c
 * @brief Container per lo stato del supervisore di sistema.
 * @details Include maschere di errore (critical/degraded) e azioni correnti.
 * È fondamentale per coordinare la logica di fallback e le protezioni.
 */

#include "snapshot/supervisor_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

/** @brief Variabile globale statica che contiene l'ultimo stato valido del supervisore. */
static SupervisorSnapshot_t snapshot;
/** @brief Mutex per garantire l'accesso esclusivo alla variabile snapshot. */
static osMutexId_t snapshot_mutex = NULL;

/**
 * @brief Inizializza il Mutex per lo snapshot del supervisore.
 * @param mutex_handle Handle del mutex creato dal kernel CMSIS-RTOS.
 */
void SupervisorSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive i dati del supervisore in modo atomico.
 * @param src Puntatore alla struttura dati aggiornata dal task Supervisore.
 */
void SupervisorSnapshot_Write(const SupervisorSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

/**
 * @brief Legge lo stato attuale del supervisore in modo atomico.
 * @param dst Puntatore alla struttura di destinazione (tipicamente nel task Log o Controllo).
 */
void SupervisorSnapshot_Read(SupervisorSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

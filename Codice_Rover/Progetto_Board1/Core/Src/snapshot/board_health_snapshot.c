/**
 * @file board_health_snapshot.c
 * @brief Gestione thread-safe dello snapshot dei parametri vitali della scheda.
 * @details Fornisce un'interfaccia protetta da Mutex per leggere e scrivere i dati
 * relativi a temperatura e tensione batteria.
 */

#include "log/wcet_monitor.h"
#include "main.h"
#include "snapshot/board_health_snapshot.h"

/** @brief Variabile globale statica che contiene l'ultimo stato valido della salute scheda. */
static BoardHealthSnapshot_t snapshot;
/** @brief Mutex per garantire l'accesso esclusivo alla variabile snapshot. */
static osMutexId_t snapshot_mutex = NULL;

/**
 * @brief Inizializza il Mutex per lo snapshot della salute scheda.
 * @param mutex_handle Handle del mutex creato dal kernel CMSIS-RTOS.
 */
void BoardHealthSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive i dati di salute della scheda in modo atomico.
 * @param src Puntatore alla struttura dati aggiornata dal task Board Health.
 */
void BoardHealthSnapshot_Write(const BoardHealthSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;    /* Copia integrale della struttura */
        (void)osMutexRelease(snapshot_mutex);
    }
}

/**
 * @brief Legge lo stato attuale della salute scheda in modo atomico.
 * @param dst Puntatore alla struttura di destinazione (tipicamente nel task Log o Controllo).
 */
void BoardHealthSnapshot_Read(BoardHealthSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;    /* Copia integrale della struttura */
        (void)osMutexRelease(snapshot_mutex);
    }
}

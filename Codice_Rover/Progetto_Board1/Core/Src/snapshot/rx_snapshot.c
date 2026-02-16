/**
 * @file rx_snapshot.c
 * @brief Gestione thread-safe del payload ricevuto dalla comunicazione (UART/Radio).
 * @details Mantiene lo stato dell'ultima ricezione valida e dei relativi flag di errore
 * della Board 2.
 */

#include "snapshot/rx_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

/** @brief Variabile globale statica che contiene l'ultimo payload valido ricevuto. */
static RxSnapshot_t snapshot;
/** @brief Mutex per garantire l'accesso esclusivo alla variabile snapshot. */
static osMutexId_t snapshot_mutex = NULL;

/**
 * @brief Inizializza il Mutex per lo snapshot della ricezione.
 * @param mutex_handle Handle del mutex creato dal kernel CMSIS-RTOS.
 */
void RxSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive i dati ricevuti in modo atomico.
 * @param src Puntatore alla struttura dati aggiornata dal task di ricezione.
 */
void RxSnapshot_Write(const RxSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

/**
 * @brief Legge lo stato attuale della ricezione in modo atomico.
 * @param dst Puntatore alla struttura di destinazione (tipicamente nel task Log o Supervisore).
 */
void RxSnapshot_Read(RxSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

/**
 * @file encoder_snapshot.c
 * @brief Container sicuro per i dati di velocità e feedback degli encoder.
 * @details Permette al task Encoder di pubblicare i dati e al task Controllo di
 * consumarli senza race conditions.
 */

#include "snapshot/encoder_snapshot.h"
#include "log/wcet_monitor.h"
#include "main.h"

/** @brief Variabile globale statica che contiene l'ultimo stato valido degli encoder. */
static EncoderSnapshot_t snapshot;
/** @brief Mutex per garantire l'accesso esclusivo alla variabile snapshot. */
static osMutexId_t snapshot_mutex = NULL;

/**
 * @brief Inizializza il Mutex per lo snapshot encoder.
 * @param mutex_handle Handle del mutex creato dal kernel CMSIS-RTOS.
 */
void EncoderSnapshot_MutexInit(osMutexId_t mutex_handle)
{
    if (snapshot_mutex == NULL)
    {
        snapshot_mutex = mutex_handle;
    }
}

/**
 * @brief Scrive i dati encoder in modo atomico.
 * @param src Puntatore alla struttura dati aggiornata dal task Encoder.
 */
void EncoderSnapshot_Write(const EncoderSnapshot_t *src)
{
    if (src != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        snapshot = *src;
        (void)osMutexRelease(snapshot_mutex);
    }
}

/**
 * @brief Legge lo stato attuale degli encoder in modo atomico.
 * @param dst Puntatore alla struttura di destinazione (tipicamente nel task Controllo).
 */
void EncoderSnapshot_Read(EncoderSnapshot_t *dst)
{
    if (dst != NULL)
    {
        (void)osMutexAcquire(snapshot_mutex, osWaitForever);
        *dst = snapshot;
        (void)osMutexRelease(snapshot_mutex);
    }
}

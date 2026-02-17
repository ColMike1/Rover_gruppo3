/**
 * @file board_health_task.c
 * @brief Implementazione del task periodico per il monitoraggio dei parametri vitali della board.
 * @date Jan 8, 2026
 * @author Sterm
 */

#include "board_health.h"
#include "board_health_snapshot.h"
#include "cmsis_os2.h"

/**
 * @brief Inizializza il task di Board Health.
 * @details Chiama l'inizializzazione dell'hardware ADC.
 */
void BoardHealth_TaskInit(void)
{
    BoardHealthADC_Init();
}

/**
 * @brief Step periodico per la gestione dello stato della board.
 * @details Esegue la lettura di temperatura e batteria, aggiorna i timestamp di validità
 * e scrive lo snapshot globale del sistema.
 */
void BoardHealth_TaskStep(void)
{
    static BoardHealthSnapshot_t snap;
    float temp_degC = 0.0f; /* Rule 9.1: Inizializzazione obbligatoria */
    float batt_pct = 0.0f;
    uint32_t now;

    BoardHealthStatus_t temp_st;
    BoardHealthStatus_t batt_st;

    /* Lettura sensori */
    temp_st = BoardHealth_ReadTemperature(&temp_degC);
    batt_st = BoardHealth_ReadBattery(&batt_pct);

    now = osKernelGetTickCount();
    snap.task_last_run_ms = now;

    /* Aggiornamento snapshot temperatura se valida */
    if (temp_st == BOARD_HEALTH_OK)
    {
        snap.temperature_degC = temp_degC;
        snap.temp_last_valid_ms = now;
    }

    /* Aggiornamento snapshot batteria se valida */
    if (batt_st == BOARD_HEALTH_OK)
    {
        snap.battery_pct = batt_pct;
        snap.batt_last_valid_ms = now;
    }

    /* Pubblicazione snapshot */
    BoardHealthSnapshot_Write(&snap);
}

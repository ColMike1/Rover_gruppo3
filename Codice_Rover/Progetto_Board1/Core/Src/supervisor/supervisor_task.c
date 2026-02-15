/**
 * @file supervisor_task.c
 * @brief Implementazione del Task Supervisore di Board 1.
 * @details Questo modulo aggrega gli snapshot di salute della scheda, encoder e
 * comunicazioni per alimentare la logica di controllo di alto livello (SupervisorB1).
 * Gestisce inoltre le uscite hardware critiche come il relè di potenza e il pin di Emergency Stop.
 * @date Feb 1, 2026
 * @author Sterm
 */

#include "supervisor/supervisor_task.h"
#include "snapshot/board_health_snapshot.h"
#include "snapshot/encoder_snapshot.h"
#include "snapshot/rx_snapshot.h"
#include "snapshot/supervisor_snapshot.h"
#include "shared_headers/supervisor_command.h"

#include "supervisorB1.h"
#include "rtwtypes.h"
#include <stdbool.h>
#include "cmsis_os2.h"

/**
 * @brief Inizializza il modello del supervisore.
 * @details Richiama la funzione di inizializzazione del supervisore, generata tramite embedded coder.
 */
void Supervisor_TaskInit(void)
{
    SupervisorB1_initialize();
}

/**
 * @brief Esegue il ciclo periodico di supervisione e gestione guasti.
 * @details
 * 1. Legge gli snapshot correnti (Health, Encoder, RX).
 * 2. Verifica la "liveliness" della Board 2 tramite l'alive_counter.
 * 3. Alimenta la struttura di input del modello 'SupervisorB1_U'.
 * 4. Esegue lo step della logica ('SupervisorB1_step').
 * 5. Attua le decisioni di sicurezza (E-Stop, Relè) e pubblica lo snapshot del supervisore.
 */
void Supervisor_TaskStep(void)
{
    static BoardHealthSnapshot_t bh;
    static EncoderSnapshot_t enc;
    static RxSnapshot_t rx;
    static SupervisorSnapshot_t sup;
    static uint8_t last_sup_counter;
    static uint32_t last_sup_update_ms;

    /* Lettura dei dati protetti da mutex */
    BoardHealthSnapshot_Read(&bh);
    EncoderSnapshot_Read(&enc);
    RxSnapshot_Read(&rx);

    uint32_t now = osKernelGetTickCount();
    CommPayloadB2_t payload = rx.payload;

    /* Monitoraggio watchdog software sulla Board 2 */
    if (payload.alive_counter != last_sup_counter)
    {
        last_sup_counter = payload.alive_counter;
        last_sup_update_ms = now;
    }

    /* Preparazione input per il modello di controllo */
    SupervisorB1_U.Board2_Data = rx;
    SupervisorB1_U.Board_Health = bh;
    SupervisorB1_U.Encoder = enc;
    SupervisorB1_U.last_valid_b2_ms = last_sup_update_ms;
    SupervisorB1_U.now_ms = now;

    /* Esecuzione logica di supervisione */
    SupervisorB1_step();

    /* Raccolta output dal modello */
    sup.critical_mask = SupervisorB1_Y.critical_mask;
    sup.degraded_mask = SupervisorB1_Y.degraded_mask;
    sup.speed_ref_rpm = SupervisorB1_Y.v_ref;
    sup.steering_cmd  = SupervisorB1_Y.omega_ref;

    sup.current_action = SupervisorB1_Y.current_action;
    sup.isBoardActuating = !SupervisorB1_Y.give_b2_actuation;

    /* --- Attuazione Hardware di Sicurezza --- */

    /** * @brief Gestione Emergency Stop hardware.
     * RESET spegne il sistema (sicurezza positiva), SET consente il movimento.
     */
    if(SupervisorB1_Y.actuate_emergency_stop)
    {
        HAL_GPIO_WritePin(ESTOP_GPIO_Port, ESTOP_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(ESTOP_GPIO_Port, ESTOP_Pin, GPIO_PIN_SET);
    }

    /** * @brief Controllo Relè di potenza.
     * Abilita o disabilita l'alimentazione ai driver motori.
     */
    if (sup.isBoardActuating)
    {
        HAL_GPIO_WritePin(RELAY_IN_GPIO_Port, RELAY_IN_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(RELAY_IN_GPIO_Port, RELAY_IN_Pin, GPIO_PIN_RESET);
    }

    /* Aggiornamento diagnostica dello snapshot */
    sup.alive_counter++;
    sup.task_last_run_ms = now;

    /* Pubblicazione dei risultati per gli altri task (Control, Led, Log) */
    SupervisorSnapshot_Write(&sup);
}

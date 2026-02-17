/**
 * @file supervisor_task.c
 * @brief Task supervisore board 2: controllo stato rover e gestione comando/attuazione.
 */

#include "supervisor_task.h"

#include "ble_controller_snapshot.h"
#include "imu_snapshot.h"
#include "sonar_snapshot.h"
#include "rx_snapshot.h"
#include "supervisor_snapshot.h"

#include "actuation_step.h"

#include "SupervisorB2.h"

#include "cmsis_os2.h"
#include "gpio.h"
#include <stdbool.h>

/**
 * @brief Inizializza il task supervisore.
 */
void Supervisor_TaskInit(void)
{
    SupervisorB2_initialize();
}

/**
 * @brief Esegue uno step del supervisore.
 *
 * In ogni ciclo legge tutti gli snapshot, valuta freschezza/timeout dei dati
 * (stato normale, degradato, critico), integra lo stato locale/remoto del rover
 * e aggiorna il comando con i riferimenti di velocita' da applicare/trasmettere
 * (es. riduzione in degrado, schivata sonar, stop di emergenza).
 */
void Supervisor_TaskStep(void)
{
    static SonarSnapshot_t son;
    static IMUSnapshot_t imu;
    static BleControllerSnapshot_t ble;
    static RxSnapshot_t rx;
    static SupervisorSnapshot_t sup;
    static uint8_t last_sup_counter = 0U;
    static uint32_t last_sup_update_ms = 0U;
    static bool actuation_initialized = false;

    /* Lettura snapshot prodotti dagli altri task. */
    SonarSnapshot_Read(&son);
    IMUSnapshot_Read(&imu);
    BleControllerSnapshot_Read(&ble);
    RxSnapshot_Read(&rx);

    uint32_t now = osKernelGetTickCount();

    /*
     * Stima di "freschezza" dei dati board 1:
     * non avendo un timestamp remoto affidabile, aggiorniamo il riferimento
     * locale quando cambia alive_counter.
     */
    if (rx.payload.alive_counter != last_sup_counter)
    {
        last_sup_counter = rx.payload.alive_counter;
        last_sup_update_ms = now;
    }

    /* Aggancio ingressi e dati temporali allo step supervisore. */
    SupervisorB2_U.Board1_Data = rx;
    SupervisorB2_U.BLE = ble;
    SupervisorB2_U.IMU = imu;
    SupervisorB2_U.Sonars = son;
    SupervisorB2_U.last_valid_b1_ms = last_sup_update_ms;
    SupervisorB2_U.now_ms = now;

    /* Esegue la logica supervisore e produce stato/mask/riferimenti di comando. */
    SupervisorB2_step();

    /* Copia uscite modello nello snapshot supervisore condiviso. */
    sup.critical_mask = SupervisorB2_Y.critical_mask;
    sup.degraded_mask = SupervisorB2_Y.degraded_mask;
    sup.command = SupervisorB2_Y.B2Decision;
    sup.isMotionConsistent = SupervisorB2_Y.isMotionConsistent;
    sup.alive_counter++;
    sup.task_last_run_ms = now;

    /* Azioni firmware non modellate direttamente nel chart. */
    bool authorized_to_send_command = SupervisorB2_Y.autorized_to_send_command;
    if (authorized_to_send_command)
    {
        if (!actuation_initialized)
        {
            Actuation_Init();
            actuation_initialized = true;
        }
        else
        {
            float v_ref = SupervisorB2_Y.v_ref_actuation;
            float omega_ref = SupervisorB2_Y.omega_ref_actuation;

            /* Applicazione riferimento all'attuatore. */
            Actuation_Step(v_ref, omega_ref);

            bool emergency_stop_requested = SupervisorB2_Y.actuate_emergency_stop;
            if (emergency_stop_requested)
            {
                HAL_GPIO_WritePin(ESTOP_GPIO_Port, ESTOP_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(ESTOP_GPIO_Port, ESTOP_Pin, GPIO_PIN_SET);
            }
        }
    }

    SupervisorSnapshot_Write(&sup);
}

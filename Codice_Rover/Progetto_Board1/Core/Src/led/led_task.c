/**
 * @file led_task.c
 * @brief Gestione della segnalazione visiva tramite LED di stato.
 * @details Controlla i LED per indicare errori critici (Rosso fisso), stati degradati
 * (Rosso lampeggiante), direzioni di marcia (Giallo) ed Emergency Stop (Blu).
 */

#include "led/led_task.h"
#include "snapshot/supervisor_snapshot.h"
#include "cmsis_os2.h"
#include "string.h"
#include "gpio.h"
#include "snapshot/rx_snapshot.h"

/** @brief Contatore per il timing del lampeggio (utilizzato per lo stato degraded). */
static int n = 0;

/**
 * @brief Inizializza i pin GPIO dei LED allo stato spento.
 */
void Led_TaskInit(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Step periodico del task LED.
 * @details Legge gli snapshot di sistema e aggiorna le uscite GPIO in base alla logica di stato.
 */
void Led_TaskStep(void)
{
    static SupervisorSnapshot_t sup;
    static RxSnapshot_t rx;

    SupervisorSnapshot_Read(&sup);
    RxSnapshot_Read(&rx);

    /* --- Gestione LED Rosso (Stato di errore) --- */
    if ((sup.critical_mask != 0U) || (rx.payload.critical_mask != 0U))
    {
        /* Errore critico: LED fisso */
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
    }
    else if ((sup.degraded_mask != 0U) || (rx.payload.degraded_mask != 0U))
    {
        /* Lampeggio in caso di stato degraded (ogni 5 chiamate del task) */
        if (n >= 5)
        {
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
            n = 0;
        }
        n++;
    }
    else
    {
        /* Nessun errore: LED spento */
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
        n = 0;
    }

    /* --- Gestione LED Giallo Sinistro (Indicatori di direzione) --- */
    if (sup.current_action == CMD_GO_LEFT)
    {
        HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(YELLOW_LED_LEFT_GPIO_Port, YELLOW_LED_LEFT_Pin, GPIO_PIN_RESET);
    }

    /* --- Gestione LED Giallo Destro --- */
    if (sup.current_action == CMD_GO_RIGHT)
    {
        HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(YELLOW_LED_RIGHT_GPIO_Port, YELLOW_LED_RIGHT_Pin, GPIO_PIN_RESET);
    }

    /* --- Gestione LED Blu (E-Stop) --- */
    if (sup.current_action == CMD_ESTOP)
    {
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
    }
}

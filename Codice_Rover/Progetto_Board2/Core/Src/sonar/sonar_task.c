/**
 * @file sonar_task.c
 * @brief Task sonar e callback ISR di acquisizione capture.
 */

#include "sonar/sonar_task.h"
#include "sonar/sonar_hw.h"
#include "snapshot/sonar_snapshot.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

/** @brief Numero di canali sonar gestiti (uno per sonar). */
#define SONAR_CHANNEL_COUNT 3U

/** @brief Handle del task sonar, usato per la notifica da ISR. */
static TaskHandle_t sonarTaskHandle = NULL;

/** @brief Stato validita' canali aggiornato da ISR capture. */
extern sonars_flag_t flag;
/** @brief Numero di canali che hanno completato la misura corrente. */
extern uint8_t sonar_count;

/**
 * @brief Inizializza il task sonar.
 */
void Sonar_TaskInit(void)
{
    sonarTaskHandle = xTaskGetCurrentTaskHandle();
}

/**
 * @brief Esegue uno step del task sonar.
 */
void Sonar_TaskStep(void)
{
    static SonarSnapshot_t snap;

    snap.task_last_run_ms = osKernelGetTickCount();

    scan();

    SonarHW_GetDistances(&snap.dist_cm[0], &snap.dist_cm[1], &snap.dist_cm[2]);

    SonarSnapshot_Write(&snap);
}

/**
 * @brief Callback HAL su evento input-capture timer sonar.
 * @param htim Handle del timer che ha generato l'evento.
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (htim->Instance == TIM1)
    {
        switch (htim->Channel)
        {
            case HAL_TIM_ACTIVE_CHANNEL_1:
                if (flag.sonar1_ok == 0U)
                {
                    flag.sonar1_ok = 1U;
                    sonar_count++;
                }
                break;

            case HAL_TIM_ACTIVE_CHANNEL_2:
                if (flag.sonar2_ok == 0U)
                {
                    flag.sonar2_ok = 1U;
                    sonar_count++;
                }
                break;

            case HAL_TIM_ACTIVE_CHANNEL_3:
                if (flag.sonar3_ok == 0U)
                {
                    flag.sonar3_ok = 1U;
                    sonar_count++;
                }
                break;

            default:
                break;
        }
    }

    if ((sonar_count >= SONAR_CHANNEL_COUNT) && (sonarTaskHandle != NULL))
    {
        /* Notifica task sonar quando tutte le capture sono arrivate. */
        xTaskNotifyFromISR(sonarTaskHandle, 0U, eNoAction, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

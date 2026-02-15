/**
 * @file sonar_hw.c
 * @brief Gestione hardware sonar con cattura input tramite timer/DMA.
 */

#include "sonar/sonar_hw.h"
#include "tim.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

/** @brief Timeout massimo per una scansione sonar [ms]. */
#define SONAR_TIMEOUT_MS            20U
/** @brief Distanza saturata quando la misura non e' valida [cm]. */
#define SONAR_MAX_CM                301U
/** @brief Numero di campioni di cattura per canale (rising/falling). */
#define SONAR_DMA_CAPTURE_SAMPLES   2U
/** @brief Valore massimo contatore timer a 16 bit. */
#define SONAR_TIMER_MAX_TICK        0xFFFFU
/** @brief Conversione da microsecondi a centimetri. */
#define SONAR_US_TO_CM_DIV          58U

/** @brief Stato di validita' delle tre misure sonar. */
sonars_flag_t flag;
/** @brief Ultime distanze calcolate. */
distances_t distances;
/** @brief Buffer DMA di cattura per i tre canali sonar. */
buffer_sonars_t buffers;

/** @brief Contatore dei sonar che hanno completato la misura corrente. */
uint8_t sonar_count = 0U;

/**
 * @brief Esegue una scansione sonar completa su 3 canali.
 */
void scan(void)
{
    /* Disabilita interrupt capture e reset stato misura. */
    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3);
    sonar_count = 0U;
    flag.sonar1_ok = 0U;
    flag.sonar2_ok = 0U;
    flag.sonar3_ok = 0U;

    while (ulTaskNotifyTake(pdTRUE, 0U) > 0U)
    {
        /* Svuota eventuali notifiche pendenti. */
    }

    memset(&buffers, 0, sizeof(buffers));

    /* Reinizializza i tre stream DMA capture. */
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_1);
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_2);
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_3);

    (void)HAL_TIM_IC_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)buffers.buf_ch1, SONAR_DMA_CAPTURE_SAMPLES);
    (void)HAL_TIM_IC_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t *)buffers.buf_ch2, SONAR_DMA_CAPTURE_SAMPLES);
    (void)HAL_TIM_IC_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t *)buffers.buf_ch3, SONAR_DMA_CAPTURE_SAMPLES);

    /* Trigger ultrasuoni e attesa completamento/timeout. */
    (void)HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    (void)ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(SONAR_TIMEOUT_MS));

    /* Stop rigoroso per evitare interrupt spuri dopo la finestra di misura. */
    (void)HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_1);
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_2);
    (void)HAL_TIM_IC_Stop_DMA(&htim1, TIM_CHANNEL_3);

    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3);

    /* Calcolo distanza; se canale non valido applica saturazione. */
    distances.distance1 = (flag.sonar1_ok != 0U) ? read_distance(buffers.buf_ch1) : SONAR_MAX_CM;
    distances.distance2 = (flag.sonar2_ok != 0U) ? read_distance(buffers.buf_ch2) : SONAR_MAX_CM;
    distances.distance3 = (flag.sonar3_ok != 0U) ? read_distance(buffers.buf_ch3) : SONAR_MAX_CM;
}

/**
 * @brief Converte la larghezza impulso catturata in distanza [cm].
 * @param buf Buffer con due capture consecutive (start/end).
 * @return Distanza in centimetri.
 */
uint32_t read_distance(uint16_t *buf)
{
    uint16_t ic1 = buf[0U];
    uint16_t ic2 = buf[1U];
    uint16_t width;

    if (ic2 >= ic1)
    {
        width = ic2 - ic1;
    }
    else
    {
        width = (uint16_t)((SONAR_TIMER_MAX_TICK - ic1) + ic2 + 1U);
    }

    /* Con timer a 1 MHz: width e' in us, distanza cm = us / 58. */
    return (uint32_t)(width / SONAR_US_TO_CM_DIV);
}

/**
 * @brief Restituisce le ultime distanze sonar disponibili.
 * @param d1 Distanza canale 1.
 * @param d2 Distanza canale 2.
 * @param d3 Distanza canale 3.
 */
void SonarHW_GetDistances(uint16_t *d1, uint16_t *d2, uint16_t *d3)
{
    if (d1 != NULL) { *d1 = distances.distance1; }
    if (d2 != NULL) { *d2 = distances.distance2; }
    if (d3 != NULL) { *d3 = distances.distance3; }
}

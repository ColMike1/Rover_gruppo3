/**
 * @file board_health_adc.c
 * @brief Gestione dei canali ADC per il monitoraggio dello stato della scheda (Temperatura e Batteria).
 * @date Jan 8, 2026
 * @author Sterm
 */

#include "board_health/board_health_adc.h"
#include "stm32g4xx_hal.h"
#include <stddef.h> /* Per NULL */

/** @brief Handle dell'ADC1 generato da CubeMX per la scansione dei canali. */
extern ADC_HandleTypeDef hadc1;

/* ---- Costanti hardware (Rule 12.1: Uso di parentesi per chiarezza) ---- */
/** @brief Risoluzione massima ADC (12-bit). */
#define ADC_MAX_COUNTS    (4095.0f)
/** @brief Limite inferiore temperatura operativa [°C]. */
#define TEMP_MIN_DEGC     (-40.0f)
/** @brief Limite superiore temperatura operativa [°C]. */
#define TEMP_MAX_DEGC     (125.0f)
/** @brief Soglia minima tensione batteria per validità lettura [V]. */
#define BATT_MIN_V        (7.0f)
/** @brief Soglia massima tensione batteria ammessa [V]. */
#define BATT_MAX_V        (15.0f)
/** @brief Guadagno del partitore di tensione esterno. */
#define BATT_DIVIDER_GAIN ((14700.0f + 4700.0f) / 4700.0f)

/* Indirizzi di calibrazione STM32G4 */
/** @brief Indirizzo di memoria del valore di calibrazione VREFINT. */
#define VREFINT_CAL_ADDR_VAL    ((uint16_t *)((uint32_t)0x1FFF75AAU))
/** @brief Indirizzo di calibrazione sensore temperatura a 30°C. */
#define TSENSE_CAL1_ADDR_VAL    ((uint16_t *)((uint32_t)0x1FFF75A8U))
/** @brief Indirizzo di calibrazione sensore temperatura a 110°C. */
#define TSENSE_CAL2_ADDR_VAL    ((uint16_t *)((uint32_t)0x1FFF75CAU))

/* ---------- Moving average state (file-local) ---------- */
/** @brief Dimensione della finestra per la media mobile della temperatura. */
#define TEMP_MA_WINDOW  10U
/** @brief Dimensione della finestra per la media mobile della batteria. */
#define BATT_MA_WINDOW  10U

/* Mappatura fissa */
/** @brief Indice del canale VREF interno nel buffer DMA. */
#define ADC_RAW_VREF   0U
/** @brief Indice del canale Sensore di Temperatura nel buffer DMA. */
#define ADC_RAW_TEMP   1U
/** @brief Indice del canale Tensione Batteria nel buffer DMA. */
#define ADC_RAW_BATT   2U

/** @brief Macro per ottenere la dimensione della LUT batteria. */
#define BATT_LUT_SIZE (sizeof(batt_pct_lut)/sizeof(batt_pct_lut[0]))

/** @brief Buffer per la media mobile della temperatura. */
static float temp_ma_buf[TEMP_MA_WINDOW];
/** @brief Buffer per la media mobile della tensione batteria. */
static float batt_ma_buf[BATT_MA_WINDOW];
static uint8_t temp_ma_idx = 0U;
static uint8_t temp_ma_cnt = 0U;
static uint8_t batt_ma_idx = 0U;
static uint8_t batt_ma_cnt = 0U;

/* ---------- LUT batteria ---------- */
/** @brief Lookup table per le percentuali della batteria. */
static const float batt_pct_lut[]  = { 0.0f, 10.0f, 20.0f, 30.0f, 45.0f, 65.0f, 80.0f, 90.0f, 100.0f };
/** @brief Lookup table per le tensioni corrispondenti alle percentuali [V]. */
static const float batt_volt_lut[] = { 9.9f, 10.5f, 10.8f, 11.1f, 11.4f, 11.7f, 12.0f, 12.3f, 12.6f };

/** @brief Array contenente i valori grezzi letti dall'ADC tramite DMA. */
static uint16_t adc_raw[3] = {0U, 0U, 0U};
static uint16_t vrefint_cal = 0U;
static uint16_t tcal1 = 0U;
static uint16_t tcal2 = 0U;

/** @brief Flag indicante la disponibilità di nuovi dati ADC. */
volatile uint8_t adc_ready = 0U;

/**
 * @brief Carica i valori di calibrazione di fabbrica dai registri di sistema STM32.
 */
static void BoardHealthADC_LoadCal(void)
{
    /* Rule 11.4: Conversione ammessa per registri di calibrazione */
    vrefint_cal = *VREFINT_CAL_ADDR_VAL;
    tcal1       = *TSENSE_CAL1_ADDR_VAL;
    tcal2       = *TSENSE_CAL2_ADDR_VAL;
}

/**
 * @brief Inizializza il sottosistema ADC per il monitoraggio board.
 * @details Carica i valori di calibrazione e avvia la scansione circolare in DMA.
 */
void BoardHealthADC_Init(void)
{
    BoardHealthADC_LoadCal();
    /* Rule 11.1: Cast per l'API HAL DMA */
    (void)HAL_ADC_Start_DMA(&hadc1, (uint32_t *)((void *)adc_raw), 3U);
}

/**
 * @brief Calcola l'aggiornamento della media mobile per un dato buffer.
 * @param buf Puntatore al buffer della media.
 * @param size Dimensione massima del buffer.
 * @param idx Puntatore all'indice corrente del buffer.
 * @param count Puntatore al conteggio degli elementi validi.
 * @param sample Nuovo campione da inserire.
 * @return float Valore medio calcolato.
 */
static float MovingAverage_Update(float * const buf, uint8_t size, uint8_t * const idx, uint8_t * const count, float sample)
{
    float sum = 0.0f;
    uint8_t i;

    buf[*idx] = sample;
    *idx = (uint8_t)((*idx + 1U) % size);

    if (*count < size)
    {
        *count = (uint8_t)(*count + 1U);
    }

    for (i = 0U; i < *count; i++)
    {
        sum += buf[i];
    }

    return (sum / (float)(*count));
}

/**
 * @brief Converte la tensione della batteria in percentuale tramite interpolazione lineare su LUT.
 * @param v Tensione misurata [V].
 * @return float Percentuale di carica residua (0.0 - 100.0).
 */
static float Battery_VoltageToPercent(float v)
{
    float result = 0.0f;

    if (v <= batt_volt_lut[0])
    {
        const float dv = batt_volt_lut[1] - batt_volt_lut[0];
        const float dp = batt_pct_lut[1]  - batt_pct_lut[0];
        result = batt_pct_lut[0] + ((dp * (v - batt_volt_lut[0])) / dv);
    }
    else if (v >= batt_volt_lut[BATT_LUT_SIZE - 1U])
    {
        const uint8_t i_max = (uint8_t)BATT_LUT_SIZE - 2U;
        const float dv = batt_volt_lut[i_max + 1U] - batt_volt_lut[i_max];
        const float dp = batt_pct_lut[i_max + 1U]  - batt_pct_lut[i_max];
        result = batt_pct_lut[i_max] + ((dp * (v - batt_volt_lut[i_max])) / dv);
    }
    else
    {
        uint8_t i;
        for (i = 0U; i < ((uint8_t)BATT_LUT_SIZE - 1U); i++)
        {
            if ((v >= batt_volt_lut[i]) && (v <= batt_volt_lut[i + 1U]))
            {
                const float dv = batt_volt_lut[i + 1U] - batt_volt_lut[i];
                const float dp = batt_pct_lut[i + 1U]  - batt_pct_lut[i];
                result = batt_pct_lut[i] + ((dp * (v - batt_volt_lut[i])) / dv);
                break;
            }
        }
    }

    if (result > 100.0f) { result = 100.0f; }
    if (result < 0.0f)   { result = 0.0f; }

    return result;
}

/**
 * @brief Legge ed elabora la temperatura interna del microcontrollore.
 * @details Utilizza VREFINT per compensare fluttuazioni di VDDA e applica i parametri di calibrazione.
 * @param[out] temp_degC Puntatore dove scrivere il valore di temperatura calcolato.
 * @return BoardHealthStatus_t OK se la lettura è valida, errore altrimenti.
 */
BoardHealthStatus_t BoardHealth_ReadTemperature(float * const temp_degC)
{
    BoardHealthStatus_t status = BOARD_HEALTH_OK;

    if (temp_degC == NULL)
    {
        status = BOARD_HEALTH_ADC_ERROR;
    }
    else
    {
        uint32_t raw_vref;
        uint32_t raw_temp;

        __disable_irq();
        raw_vref = (uint32_t)adc_raw[ADC_RAW_VREF];
        raw_temp = (uint32_t)adc_raw[ADC_RAW_TEMP];
        __enable_irq();

        if (raw_vref == 0U)
        {
            status = BOARD_HEALTH_ADC_ERROR;
        }
        else
        {
            const float vdda = 3.0f * ((float)vrefint_cal / (float)raw_vref);
            const float adc_temp_at_3v = (float)raw_temp * (vdda / 3.0f);
            const float temp_raw = (80.0f * (adc_temp_at_3v - (float)tcal1) / (float)((uint32_t)tcal2 - (uint32_t)tcal1)) + 30.0f;
            const float temp_filt = MovingAverage_Update(temp_ma_buf, TEMP_MA_WINDOW, &temp_ma_idx, &temp_ma_cnt, temp_raw);

            if ((temp_filt < TEMP_MIN_DEGC) || (temp_filt > TEMP_MAX_DEGC))
            {
                status = BOARD_HEALTH_OUT_OF_RANGE;
            }
            else
            {
                *temp_degC = temp_filt;
            }
        }
    }
    return status;
}

/**
 * @brief Legge ed elabora la tensione della batteria convertendola in percentuale.
 * @param[out] battery_pct Puntatore dove scrivere la percentuale calcolata.
 * @return BoardHealthStatus_t OK se la lettura è valida, errore altrimenti.
 */
BoardHealthStatus_t BoardHealth_ReadBattery(float * const battery_pct)
{
    BoardHealthStatus_t status = BOARD_HEALTH_OK;

    if (battery_pct == NULL)
    {
        status = BOARD_HEALTH_ADC_ERROR;
    }
    else
    {
        uint32_t raw_vref;
        uint32_t raw_batt;

        __disable_irq();
        raw_vref = (uint32_t)adc_raw[ADC_RAW_VREF];
        raw_batt = (uint32_t)adc_raw[ADC_RAW_BATT];
        __enable_irq();

        if (raw_vref == 0U)
        {
            status = BOARD_HEALTH_ADC_ERROR;
        }
        else
        {
            const float vdda = 3.0f * ((float)vrefint_cal / (float)raw_vref);
            const float v_batt_adc = ((float)raw_batt * vdda) / ADC_MAX_COUNTS;
            const float v_batt = v_batt_adc * BATT_DIVIDER_GAIN;

            if ((v_batt < BATT_MIN_V) || (v_batt > BATT_MAX_V))
            {
                status = BOARD_HEALTH_OUT_OF_RANGE;
            }
            else
            {
                const float v_filt = MovingAverage_Update(batt_ma_buf, BATT_MA_WINDOW, &batt_ma_idx, &batt_ma_cnt, v_batt);
                *battery_pct = Battery_VoltageToPercent(v_filt);
            }
        }
    }
    return status;
}

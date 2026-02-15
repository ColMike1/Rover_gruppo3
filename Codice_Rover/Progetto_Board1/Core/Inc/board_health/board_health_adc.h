/**
 * @file board_health_adc.h
 * @brief Gestione dei parametri vitali della scheda tramite ADC.
 */

#ifndef INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_
#define INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_

#include <stdint.h>

/**
 * @brief Enumerazione per lo stato di salute della scheda.
 */
typedef enum
{
  BOARD_HEALTH_OK = 0,        /**< Operazione completata con successo */
  BOARD_HEALTH_ADC_ERROR,     /**< Errore hardware o di lettura ADC */
  BOARD_HEALTH_OUT_OF_RANGE   /**< Valore letto fuori dai limiti di sicurezza */
} BoardHealthStatus_t;

/**
 * @brief Legge il sensore di temperatura interno della MCU.
 * @param[out] temp_degC Puntatore alla variabile dove salvare la temperatura in gradi Celsius.
 * @return BoardHealthStatus_t Stato della lettura.
 */
BoardHealthStatus_t BoardHealth_ReadTemperature(float *temp_degC);

/**
 * @brief Legge il livello di tensione della batteria.
 * @param[out] battery_pct Puntatore alla variabile per il livello percentuale [0..100].
 * @return BoardHealthStatus_t Stato della lettura.
 */
BoardHealthStatus_t BoardHealth_ReadBattery(float *battery_pct);

/**
 * @brief Inizializza l'ADC e i canali dedicati al monitoraggio della scheda.
 */
void BoardHealthADC_Init(void);

#endif /* INC_BOARD_HEALTH_BOARD_HEALTH_ADC_H_ */

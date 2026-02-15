/**
 * @file sabertooth_driver.h
 * @brief Driver per i controller motori Sabertooth.
 */

#ifndef INC_ACTUATION_SABERTOOTH_DRIVER_H_
#define INC_ACTUATION_SABERTOOTH_DRIVER_H_

#include <stdint.h>

/** * @name Configurazione Sabertooth
 * @{
 */
#define SABER_MAX_SPEED   127 /**< Massima velocità impostabile per il driver */

#define SABER_FRONT_ADDR       135 /**< Indirizzo del controller motori anteriore */
#define SABER_BACK_ADDR       134  /**< Indirizzo del controller motori posteriore */
/** @} */

/**
 * @brief Inizializza la periferica di comunicazione per i controller Sabertooth.
 */
void Sabertooth_Init(void);

/**
 * @brief Invia i comandi di velocità ai 4 motori.
 * @param usx_p Velocità motore posteriore sinistro (RPM).
 * @param udx_p Velocità motore posteriore destro (RPM).
 * @param usx_a Velocità motore anteriore sinistro (RPM).
 * @param udx_a Velocità motore anteriore destro (RPM).
 */
void Sabertooth_ApplyOutputs(float usx_p,
                             float udx_p,
                             float usx_a,
                             float udx_a);

/**
 * @brief Callback per la gestione degli eventi legati alla comunicazione Sabertooth.
 */
void SabertoothCallback(void);

#endif /* INC_ACTUATION_SABERTOOTH_DRIVER_H_ */

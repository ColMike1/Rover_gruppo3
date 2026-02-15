/**
 * @file sabertooth_driver.h
 * @brief Driver Sabertooth: composizione pacchetti e invio comandi motore.
 */

#ifndef INC_ACTUATION_SABERTOOTH_DRIVER_H_
#define INC_ACTUATION_SABERTOOTH_DRIVER_H_

#include <stdint.h>

/** @brief Valore massimo del comando velocita' accettato dal Sabertooth. */
#define SABER_MAX_SPEED   127

/** @brief Indirizzo seriale del Sabertooth anteriore. */
#define SABER_FRONT_ADDR       135
/** @brief Indirizzo seriale del Sabertooth posteriore. */
#define SABER_BACK_ADDR       134

/** @brief Procedura di inizializzazione dei driver Sabertooth. */
void Sabertooth_Init(void);

/**
 * @brief Invia i comandi ai quattro motori.
 * @param usx_p Comando ruota sinistra posteriore.
 * @param udx_p Comando ruota destra posteriore.
 * @param usx_a Comando ruota sinistra anteriore.
 * @param udx_a Comando ruota destra anteriore.
 */
void Sabertooth_ApplyOutputs(float usx_p, float udx_p, float usx_a, float udx_a);

/** @brief Callback chiamata al completamento della TX UART verso Sabertooth. */
void SabertoothCallback(void);
#endif /* INC_ACTUATION_SABERTOOTH_DRIVER_H_ */


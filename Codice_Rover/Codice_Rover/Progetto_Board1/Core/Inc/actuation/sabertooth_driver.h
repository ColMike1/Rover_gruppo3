/*
 * sabertooth_driver.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_ACTUATION_SABERTOOTH_DRIVER_H_
#define INC_ACTUATION_SABERTOOTH_DRIVER_H_

#include <stdint.h>

/* === Configurazione Sabertooth === */
#define SABER_MAX_SPEED   127

/* Indirizzi (adatta se necessario) */
#define SABER_FRONT_ADDR       135
#define SABER_BACK_ADDR       134

/* API driver */
void Sabertooth_Init(void);

/* Invio comandi ai 4 motori (unità fisiche: RPM firmati) */
void Sabertooth_ApplyOutputs(float usx_p,
                             float udx_p,
                             float usx_a,
                             float udx_a);


void SabertoothCallback(void);
#endif /* INC_ACTUATION_SABERTOOTH_DRIVER_H_ */

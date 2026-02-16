/**
 * @file sonar_hw.h
 * @brief API hardware sonar e strutture di supporto.
 */

#ifndef INC_SONARS_H_
#define INC_SONARS_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "cmsis_os2.h"

/** @brief Buffer DMA dei canali sonar usati per il calcolo distanza. */
typedef struct {
    uint16_t buf_ch1[2];
    uint16_t buf_ch2[2];
    uint16_t buf_ch3[2];
} buffer_sonars_t;

/** @brief Distanze sonar espresse in centimetri. */
typedef struct {
    uint16_t distance1;
    uint16_t distance2;
    uint16_t distance3;
} distances_t;

/** @brief Flag di validita' acquisizione per ciascun sonar. */
typedef struct {
    uint8_t sonar1_ok;
    uint8_t sonar2_ok;
    uint8_t sonar3_ok;
} sonars_flag_t;

/**
 * @brief Restituisce le ultime distanze sonar disponibili.
 * @param d1 Puntatore alla distanza sonar 1.
 * @param d2 Puntatore alla distanza sonar 2.
 * @param d3 Puntatore alla distanza sonar 3.
 */
void SonarHW_GetDistances(uint16_t *d1, uint16_t *d2, uint16_t *d3);

/**
 * @brief Avvia una scansione sonar.
 */
void scan(void);

/**
 * @brief Converte un buffer di cattura timer in distanza.
 * @param buf Buffer campioni della cattura input.
 * @return Distanza convertita in unita' interne.
 */
uint32_t read_distance(uint16_t *buf);

#endif /* INC_SONARS_H_ */

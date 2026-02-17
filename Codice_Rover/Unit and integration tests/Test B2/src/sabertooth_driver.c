/**
 * @file sabertooth_driver.c
 * @brief Driver Sabertooth: composizione pacchetti e invio comandi motore.
 */


#include "sabertooth_driver.h"
#include "stm32_hal.h"
#include <math.h>

/** @brief Lunghezza di un singolo pacchetto Sabertooth [byte]. */
#define SABER_PACKET_LEN            4U
/** @brief Numero pacchetti inviati durante init. */
#define SABER_INIT_PACKET_COUNT     2U
/** @brief Valore comando per impostare il timeout seriale. */
#define SABER_INIT_CMD              14U
/** @brief Timeout fail-safe in step da 100 ms (3 -> blocco motori dopo ~300 ms senza segnale). */
#define SABER_TIMEOUT_VALUE            3.0f
/** @brief Valore assoluto tensione di riferimento [V]. */
#define SABER_REF_VOLTAGE           12.0f
/** @brief Scala massima comando seriale motore. */
#define SABER_COMMAND_SCALE         127.0f
/** @brief Comando Sabertooth: motore 1 avanti. */
#define SABER_CMD_M1_FWD            0U
/** @brief Comando Sabertooth: motore 1 indietro. */
#define SABER_CMD_M1_REV            1U
/** @brief Comando Sabertooth: motore 2 avanti. */
#define SABER_CMD_M2_FWD            4U
/** @brief Comando Sabertooth: motore 2 indietro. */
#define SABER_CMD_M2_REV            5U

/** @brief Handle UART usata per la comunicazione con Sabertooth (generata da CubeMX). */
extern UART_HandleTypeDef huart5;


/** @brief Buffer TX contenente fino a 4 pacchetti Sabertooth consecutivi. */
static uint8_t saber_tx[4U * SABER_PACKET_LEN];
/** @brief Flag busy per evitare trasmissioni concorrenti. */
static volatile uint8_t tx_busy = 0;


/**
 * @brief Calcola il checksum Sabertooth su 3 byte.
 * @param a Byte 0.
 * @param b Byte 1.
 * @param c Byte 2.
 * @return Checksum a 7 bit.
 */
static inline uint8_t Saber_Checksum(uint8_t a, uint8_t b, uint8_t c)
{
    return (a + b + c) & 0x7F;
}

/**
 * @brief Compone un pacchetto Sabertooth da 4 byte.
 * @param dest Buffer di destinazione (almeno 4 byte).
 * @param addr Indirizzo del Sabertooth.
 * @param cmd Comando seriale Sabertooth.
 * @param value Velocita' in scala Sabertooth (verra' saturata in [0, SABER_MAX_SPEED]).
 */
static void Prepare_Saber_Packet(uint8_t *dest, uint8_t addr, uint8_t cmd, float value)
{
    int speed = (int)lroundf(value);

    if (speed > SABER_MAX_SPEED)
    {
        speed = SABER_MAX_SPEED;
    }
    if (speed < 0)
    {
        speed = 0;
    }

    dest[0U] = addr;
    dest[1U] = cmd;
    dest[2U] = (uint8_t)speed;
    dest[3U] = Saber_Checksum(dest[0U], dest[1U], dest[2U]);
}

/**
 * @brief Invia un breve comando di inizializzazione ai due Sabertooth.
 */
void Sabertooth_Init(void)
{
    if (tx_busy)
    {
        return;
    }

    /* Prepara i pacchetti di init per i due controller. */
    Prepare_Saber_Packet(&saber_tx[0U], SABER_BACK_ADDR, SABER_INIT_CMD, SABER_TIMEOUT_VALUE);
    Prepare_Saber_Packet(&saber_tx[SABER_PACKET_LEN], SABER_FRONT_ADDR, SABER_INIT_CMD, SABER_TIMEOUT_VALUE);

    tx_busy = 1U;

    if (HAL_UART_Transmit_IT(&huart5, saber_tx, SABER_PACKET_LEN * SABER_INIT_PACKET_COUNT) != HAL_OK)
    {
        tx_busy = 0U;
    }
}



/**
 * @brief Converte e invia i comandi ruota ai quattro canali Sabertooth.
 *
 * I valori in volt vengono convertiti in scala 0..127 usando 12 V come riferimento.
 *
 * @param usx_p Comando ruota sinistra posteriore.
 * @param udx_p Comando ruota destra posteriore.
 * @param usx_a Comando ruota sinistra anteriore.
 * @param udx_a Comando ruota destra anteriore.
 */
void Sabertooth_ApplyOutputs(float usx_p, float udx_p, float usx_a, float udx_a)
{
    if (tx_busy)
    {
        return;
    }

    float s_sx_p = (fabsf(usx_p) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    float s_dx_p = (fabsf(udx_p) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    float s_sx_a = (fabsf(usx_a) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    float s_dx_a = (fabsf(udx_a) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;

    Prepare_Saber_Packet(&saber_tx[0U], SABER_BACK_ADDR, (usx_p >= 0.0f ? SABER_CMD_M1_FWD : SABER_CMD_M1_REV), s_sx_p);
    Prepare_Saber_Packet(&saber_tx[SABER_PACKET_LEN], SABER_BACK_ADDR, (udx_p >= 0.0f ? SABER_CMD_M2_FWD : SABER_CMD_M2_REV), s_dx_p);
    Prepare_Saber_Packet(&saber_tx[2U * SABER_PACKET_LEN], SABER_FRONT_ADDR, (usx_a >= 0.0f ? SABER_CMD_M1_FWD : SABER_CMD_M1_REV), s_sx_a);
    Prepare_Saber_Packet(&saber_tx[3U * SABER_PACKET_LEN], SABER_FRONT_ADDR, (udx_a >= 0.0f ? SABER_CMD_M2_FWD : SABER_CMD_M2_REV), s_dx_a);

    tx_busy = 1U;
    if (HAL_UART_Transmit_IT(&huart5, saber_tx, sizeof(saber_tx)) != HAL_OK)
    {
        tx_busy = 0U;
    }
}

/**
 * @brief Callback chiamata al completamento della TX UART verso Sabertooth. 
 */
void SabertoothCallback()
{
    tx_busy = 0U;
}

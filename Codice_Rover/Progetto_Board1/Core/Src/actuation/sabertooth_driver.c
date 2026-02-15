/**
 * @file sabertooth_driver.c
 * @brief Driver per la gestione dei motori tramite controller Sabertooth in modalità seriale a pacchetti.
 * @date Jan 12, 2026
 * @author Sterm
 */

#include "actuation/sabertooth_driver.h"
#include "stm32g4xx_hal.h"
#include <math.h>
#include "cmsis_os.h"

/** @brief Lunghezza di un pacchetto Sabertooth [byte]. */
#define SABER_PACKET_LEN              4U
/** @brief Dimensione del buffer TX [byte]. */
#define SABER_TX_BUFFER_LEN           (4U * SABER_PACKET_LEN)
/** @brief Delay iniziale per permettere il boot dei driver [ms]. */
#define SABER_INIT_DELAY_MS           1000U
/** @brief Comando seriale per configurare il timeout Sabertooth. */
#define SABER_INIT_CMD_TIMEOUT        14U
/** @brief Valore timeout in step da 100 ms. */
#define SABER_INIT_TIMEOUT_VALUE      3.0f
/** @brief Riferimento tensione per scalatura [V]. */
#define SABER_REF_VOLTAGE             12.0f
/** @brief Massima ampiezza comando Sabertooth. */
#define SABER_COMMAND_SCALE           127.0f
/** @brief Comando Sabertooth: motore 1 avanti. */
#define SABER_CMD_M1_FWD              0U
/** @brief Comando Sabertooth: motore 1 indietro. */
#define SABER_CMD_M1_REV              1U
/** @brief Comando Sabertooth: motore 2 avanti. */
#define SABER_CMD_M2_FWD              4U
/** @brief Comando Sabertooth: motore 2 indietro. */
#define SABER_CMD_M2_REV              5U

/** @brief Handle della periferica UART utilizzata per la comunicazione con i driver. */
extern UART_HandleTypeDef huart5;

/** @brief Buffer di trasmissione per i pacchetti seriali (4 pacchetti da 4 byte ciascuno). */
static uint8_t saber_tx[SABER_TX_BUFFER_LEN];

/** @brief Flag di stato per indicare se una trasmissione DMA/Interrupt è in corso. */
static volatile uint8_t tx_busy = 0U;

/**
 * @brief Calcola il checksum a 7-bit richiesto dal protocollo Sabertooth.
 * @param a Indirizzo del dispositivo.
 * @param b Numero del comando.
 * @param c Valore del dato (velocità).
 * @return uint8_t Il checksum calcolato (somma dei byte mascherata a 0x7F).
 */
static uint8_t Saber_Checksum(uint8_t a, uint8_t b, uint8_t c)
{
    /* Somma protetta da overflow tramite casting a 32 bit */
    unsigned int sum = (unsigned int)a + (unsigned int)b + (unsigned int)c;
    return (uint8_t)(sum & 0x7FU);
}

/**
 * @brief Prepara un singolo pacchetto da 4 byte per il protocollo Sabertooth.
 * @details Gestisce il clipping del valore di velocità e il calcolo del checksum.
 * @param dest Puntatore all'array di destinazione (almeno 4 byte).
 * @param addr Indirizzo del driver Sabertooth.
 * @param cmd Comando da inviare (direzione/canale).
 * @param value Valore di velocità floating point da convertire in intero.
 */
static void Prepare_Saber_Packet(uint8_t *dest, uint8_t addr, uint8_t cmd, float value)
{
    int speed = (int)lroundf(value);

    if (speed > (int)SABER_MAX_SPEED)
    {
        speed = (int)SABER_MAX_SPEED;
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
 * @brief Inizializza i driver Sabertooth.
 * @details Introduce un ritardo di avvio e configura i parametri di timeout iniziali
 * per i driver anteriori e posteriori.
 */
void Sabertooth_Init(void)
{
    uint8_t saber_timeout_pkt[SABER_PACKET_LEN];
    (void)osDelay(SABER_INIT_DELAY_MS);

    Prepare_Saber_Packet(saber_timeout_pkt, SABER_BACK_ADDR, SABER_INIT_CMD_TIMEOUT, SABER_INIT_TIMEOUT_VALUE);
    (void)HAL_UART_Transmit(&huart5, saber_timeout_pkt, SABER_PACKET_LEN, HAL_MAX_DELAY);

    Prepare_Saber_Packet(saber_timeout_pkt, SABER_FRONT_ADDR, SABER_INIT_CMD_TIMEOUT, SABER_INIT_TIMEOUT_VALUE);
    (void)HAL_UART_Transmit(&huart5, saber_timeout_pkt, SABER_PACKET_LEN, HAL_MAX_DELAY);
}

/**
 * @brief Converte le tensioni di comando in pacchetti seriali e avvia la trasmissione.
 * @details La funzione scala i valori di tensione (0-12V) nel range Sabertooth (0-127).
 * Se una trasmissione è già in corso, la chiamata viene ignorata per non corrompere il buffer.
 * @param usx_a Tensione motore Anteriore Sinistro.
 * @param udx_a Tensione motore Anteriore Destro.
 * @param usx_p Tensione motore Posteriore Sinistro.
 * @param udx_p Tensione motore Posteriore Destro.
 */
void Sabertooth_ApplyOutputs(float usx_a, float udx_a, float usx_p, float udx_p)
{
    float s_sx_p;
    float s_dx_p;
    float s_sx_a;
    float s_dx_a;

    if (tx_busy != 0U)
    {
        return;
    }

    s_sx_p = (fabsf(usx_p) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    s_dx_p = (fabsf(udx_p) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    s_sx_a = (fabsf(usx_a) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;
    s_dx_a = (fabsf(udx_a) / SABER_REF_VOLTAGE) * SABER_COMMAND_SCALE;

    Prepare_Saber_Packet(&saber_tx[0U],                    SABER_BACK_ADDR,  ((usx_p >= 0.0f) ? SABER_CMD_M1_FWD : SABER_CMD_M1_REV), s_sx_p);
    Prepare_Saber_Packet(&saber_tx[SABER_PACKET_LEN],      SABER_BACK_ADDR,  ((udx_p >= 0.0f) ? SABER_CMD_M2_FWD : SABER_CMD_M2_REV), s_dx_p);
    Prepare_Saber_Packet(&saber_tx[2U * SABER_PACKET_LEN], SABER_FRONT_ADDR, ((usx_a >= 0.0f) ? SABER_CMD_M1_FWD : SABER_CMD_M1_REV), s_sx_a);
    Prepare_Saber_Packet(&saber_tx[3U * SABER_PACKET_LEN], SABER_FRONT_ADDR, ((udx_a >= 0.0f) ? SABER_CMD_M2_FWD : SABER_CMD_M2_REV), s_dx_a);

    tx_busy = 1U;
    if (HAL_UART_Transmit_IT(&huart5, saber_tx, (uint16_t)sizeof(saber_tx)) != HAL_OK)
    {
        tx_busy = 0U;
    }
}

/**
 * @brief Callback di completamento trasmissione UART.
 * @details Deve essere chiamata dall'ISR dell'UART (HAL_UART_TxCpltCallback)
 * per sbloccare il driver e permettere nuovi invii.
 */
void SabertoothCallback(void)
{
    tx_busy = 0U;
}

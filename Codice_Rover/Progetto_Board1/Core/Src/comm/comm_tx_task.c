/**
 * @file comm_tx_task.c
 * @brief Implementazione del task di trasmissione periodica della telemetria.
 */

#include "comm/comm_tx_task.h"
#include "comm/comm_pack.h"
#include "comm/comm_uart.h"
#include "shared_headers/comm_message_structures.h"
#include "snapshot/encoder_snapshot.h"
#include "snapshot/supervisor_snapshot.h"

/** @brief Lunghezza definita per il frame di trasmissione B1. */
#define TX_FRAME_LEN (sizeof(CommFrameB1_t))

/**
 * @brief Step periodico del task di trasmissione.
 * @details Legge gli snapshot correnti dal sistema, impacchetta i dati e invia
 * il buffer tramite il driver UART se non occupato.
 */
void Tx_TaskStep(void)
{
    EncoderSnapshot_t enc = {0};   /* Inizializzazione obbligatoria */
    SupervisorSnapshot_t sup = {0};
    static uint8_t tx_buf[sizeof(CommFrameB1_t)];
    const uint16_t tx_frame_len = (uint16_t)sizeof(CommFrameB1_t);

    EncoderSnapshot_Read(&enc);
    SupervisorSnapshot_Read(&sup);

    uint16_t tx_len = CommPack_BuildB1Tx(tx_buf, tx_frame_len, &enc, &sup);

    if (tx_len == tx_frame_len)
    {
        CommUart_Send(tx_buf, tx_len);
    }
}

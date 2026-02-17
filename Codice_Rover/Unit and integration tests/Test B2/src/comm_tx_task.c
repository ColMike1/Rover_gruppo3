/**
 * @file comm_tx_task.c
 * @brief Task TX: composizione e invio frame board2 -> board1.
 */

#include "comm_tx_task.h"
#include "comm_pack.h"
#include "comm_uart.h"

#include "comm_message_structures.h"

#include "ble_controller_snapshot.h"
#include "supervisor_snapshot.h"
#include "imu_snapshot.h"

/** @brief Lunghezza fissa del frame trasmesso verso board 1. */
#define TX_FRAME_LEN (sizeof(CommFrameB2_t))

/**
 * @brief Inizializza il task TX.
 *
 * In questa implementazione non e' richiesta inizializzazione specifica.
 */
void Tx_TaskInit(void)
{
}

/**
 * @brief Esegue uno step del task TX.
 *
 * Legge gli snapshot condivisi, costruisce il frame di trasmissione e lo invia
 * su UART solo se la lunghezza e' quella attesa.
 */
void Tx_TaskStep(void)
{
    SupervisorSnapshot_t sup;
    BleControllerSnapshot_t ble;
    IMUSnapshot_t imu;

    static uint8_t tx_buf[TX_FRAME_LEN];

    SupervisorSnapshot_Read(&sup);
    BleControllerSnapshot_Read(&ble);
    IMUSnapshot_Read(&imu);

    uint16_t tx_len = CommPack_BuildB2Tx(tx_buf, TX_FRAME_LEN, &ble, &sup, &imu);

    if (tx_len == TX_FRAME_LEN)
    {
        CommUart_Send(tx_buf, tx_len);
    }
}


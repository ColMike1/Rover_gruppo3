/**
 * @file comm_rx_task.c
 * @brief Task RX: acquisizione byte UART, riallineamento frame e unpack.
 */

#include "comm_rx_task.h"
#include "comm_unpack.h"
#include "comm_uart.h"

#include "rx_snapshot.h"
#include "cmsis_os2.h"

#include "FreeRTOS.h"
#include "task.h"

/** @brief Word di sincronizzazione del protocollo frame. */
#define SYNC_WORD   0xAA55U
/** @brief Lunghezza fissa del frame ricevuto da board 1. */
#define RX_FRAME_LEN (sizeof(CommFrameB1_t))

/**
 * @brief Inizializza il task RX.
 */
void Rx_TaskInit(void)
{
    CommUart_Init();
}

/**
 * @brief Esegue uno step RX.
 *
 * Attende dati UART, ricostruisce un frame completo, valida CRC e aggiorna
 * lo snapshot con l'ultimo payload valido.
 */
void Rx_TaskStep(void)
{
    static RxSnapshot_t snap;
    static uint8_t acc_buf[RX_FRAME_LEN];
    static uint16_t acc_len = 0U;

    uint8_t byte;

    /* Attende almeno un byte disponibile. */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    while (CommUart_GetByte(&byte))
    {
        /* Gestione sincronizzazione iniziale su 2 byte. */
        if (acc_len < sizeof(uint16_t))
        {
            acc_buf[acc_len++] = byte;

            if (acc_len == 2U)
            {
                uint16_t sync = (uint16_t)acc_buf[0U] | ((uint16_t)acc_buf[1U] << 8U);
                if (sync != SYNC_WORD)
                {
                    acc_buf[0U] = acc_buf[1U];
                    acc_len = 1U;
                }
            }
            continue;
        }

        /* Acquisizione resto frame. */
        acc_buf[acc_len] = byte;

        /* Se ricompare SYNC nel payload, riallinea il buffer. */
        uint16_t w = (uint16_t)acc_buf[acc_len - 1U] | ((uint16_t)acc_buf[acc_len] << 8U);

        if (w == SYNC_WORD)
        {
            acc_buf[0U] = acc_buf[acc_len - 1U];
            acc_buf[1U] = acc_buf[acc_len];
            acc_len = 2U;
            continue;
        }

        acc_len++;

        if (acc_len < RX_FRAME_LEN)
        {
            continue;
        }

        /* Frame completo: unpack e aggiornamento snapshot. */
        uint32_t now = osKernelGetTickCount();
        snap.task_last_run_ms = now;

        CommFrameHeader_t hdr;
        CommPayloadB1_t pl;

        CommUnpackStatus_t st = CommUnpack_B2FromB1(acc_buf, RX_FRAME_LEN, &hdr, &pl);

        snap.last_event = st;

        if (st == COMM_UNPACK_OK)
        {
            snap.payload = pl;
            snap.data_last_valid_ms = now;
        }

        acc_len = 0U;   /* Pronto per il prossimo frame. */
        RxSnapshot_Write(&snap);
    }
}

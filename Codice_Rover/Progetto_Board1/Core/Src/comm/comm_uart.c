/**
 * @file comm_uart.c
 * @brief Driver di basso livello per la comunicazione UART con gestione interrupt.
 */

#include "comm/comm_uart.h"
#include "stm32g4xx_hal.h"
#include "actuation/sabertooth_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdbool.h>

/* ===================== Configuration ===================== */
/** @brief Dimensione del buffer circolare di ricezione. */
#define UART_RX_BUF_SIZE   (256U)

/* ===================== External handles ===================== */
/** @brief Handle per la comunicazione seriale principale (Telemetria). */
extern UART_HandleTypeDef huart4;
/** @brief Handle per la comunicazione con i driver Sabertooth. */
extern UART_HandleTypeDef huart5;

/* ===================== Static data ===================== */
/** @brief Flag di stato TX per UART4. */
static volatile uint8_t uart4_tx_busy = 0U;
/** @brief Variabile di supporto per l'interrupt di ricezione (singolo byte). */
static uint8_t rx_byte = 0U;
/** @brief Buffer circolare di ricezione. */
static uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile uint16_t rx_wr = 0U;
static volatile uint16_t rx_rd = 0U;
/** @brief Handle del task da notificare all'arrivo di nuovi dati. */
static TaskHandle_t rx_task_handle = NULL;

/* ===================== Local utilities ===================== */
/**
 * @brief Incrementa l'indice del buffer circolare gestendo il wrapping.
 * @param idx Indice corrente.
 * @return uint16_t Prossimo indice.
 */
static uint16_t ring_next(uint16_t idx)
{
    uint16_t next = idx + 1U;
    if (next >= UART_RX_BUF_SIZE)
    {
        next = 0U;
    }
    return next;
}

/* ===================== API ===================== */

/**
 * @brief Registra il task che deve ricevere le notifiche di ricezione byte.
 * @param h Handle del task FreeRTOS.
 */
void CommUart_RegisterRxTask(TaskHandle_t h)
{
    rx_task_handle = h;
}

/**
 * @brief Inizializzazione iniziale del driver e avvio interrupt RX.
 */
void CommUart_EarlyInit(void)
{
    rx_wr = 0U;
    rx_rd = 0U;
    rx_task_handle = NULL;
    uart4_tx_busy = false;

    (void)HAL_UART_Receive_IT(&huart4, &rx_byte, 1U);
}

/**
 * @brief Inizializzazione standard e registrazione task chiamante per RX.
 */
void CommUart_Init(void)
{
    CommUart_RegisterRxTask(xTaskGetCurrentTaskHandle());
}

/**
 * @brief Invia un buffer di dati tramite UART4 in modalità asincrona (Interrupt).
 * @param buf Puntatore ai dati da inviare.
 * @param len Lunghezza del buffer.
 */
void CommUart_Send(const uint8_t *buf, uint16_t len)
{
    if ((buf == NULL) || (len == 0U))
    {
        return;
    }
    
    if (uart4_tx_busy != 0U)
    {
        return;
    }
    
    uart4_tx_busy = 1U;
    if (HAL_UART_Transmit_IT(&huart4, (uint8_t *)buf, len) != HAL_OK)
    {
        uart4_tx_busy = 0U;
    }
}

/* ===================== HAL callbacks ===================== */

/**
 * @brief Callback HAL chiamata al completamento della trasmissione.
 * @details Gestisce lo sblocco dei driver UART4 e UART5 (Sabertooth).
 * @param huart Handle della periferica UART.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        if(huart == &huart4)
        {
            uart4_tx_busy = 0U;
        }
        else if (huart == &huart5)
        {
            SabertoothCallback();
        }
        else
        {
            /* Caso non gestito */
        }
    }
}

/* ===================== RX API ===================== */

/**
 * @brief Estrae un byte dal buffer circolare di ricezione.
 * @param[out] b Puntatore alla variabile dove scrivere il byte estratto.
 * @return true Se un byte è stato estratto correttamente.
 * @return false Se il buffer è vuoto.
 */
bool CommUart_GetByte(uint8_t *b)
{
    bool has_data = false;

    if ((b != NULL) && (rx_rd != rx_wr))
    {
        *b = rx_buf[rx_rd];
        rx_rd = ring_next(rx_rd);
        has_data = true;
    }

    return has_data;
}

/**
 * @brief Callback HAL chiamata alla ricezione di un byte su UART4.
 * @param huart Handle della periferica UART che ha generato l'interrupt.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart4)
    {
        uint16_t next = ring_next(rx_wr);

        /* Inserisce il byte nel buffer se non pieno */
        if (next != rx_rd)
        {
            rx_buf[rx_wr] = rx_byte;
            rx_wr = next;
        }

        /* Notifica il task RX */
        if (rx_task_handle != NULL)
        {
            BaseType_t hpw = pdFALSE;
            vTaskNotifyGiveFromISR(rx_task_handle, &hpw);
            portYIELD_FROM_ISR(hpw);
        }

        /* Riarmo ricezione */
        (void)HAL_UART_Receive_IT(&huart4, &rx_byte, 1U);
    }
}



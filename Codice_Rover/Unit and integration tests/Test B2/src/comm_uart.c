/**
 * @file comm_uart.c
 * @brief Driver UART con buffer circolare per RX/TX inter-board.
 */

#include "comm_uart.h"
#include "sabertooth_driver.h"
#include "stm32_hal.h"
#include "FreeRTOS.h"
#include "task.h"
/** @brief Dimensione del buffer circolare RX UART4. */
#define UART_RX_BUF_SIZE 256U

/** @brief Handle UART4 (canale comunicazione board). */
extern UART_HandleTypeDef huart4;
/** @brief Handle UART5 (canale Sabertooth). */
extern UART_HandleTypeDef huart5;
/** @brief Flag TX occupata su UART4. */
static volatile uint8_t uart4_tx_busy = 0U;

/** @brief Byte RX temporaneo ricevuto da IRQ. */
static uint8_t rx_byte;
/** @brief Buffer circolare RX. */
uint8_t rx_buf[UART_RX_BUF_SIZE];
/** @brief Indice di scrittura del buffer RX. */
 volatile uint16_t rx_wr = 0U;
/** @brief Indice di lettura del buffer RX. */
 volatile uint16_t rx_rd = 0U;

/** @brief Handle del task da notificare alla ricezione di nuovi byte. */
static TaskHandle_t rx_task_handle = NULL;

/**
 * @brief Registra il task da notificare quando arrivano dati RX.
 * @param h Handle task RX.
 */
static void CommUart_RegisterRxTask(TaskHandle_t h)
{
    rx_task_handle = h;
}

/**
 * @brief Inizializzazione anticipata del driver UART.
 */
void CommUart_EarlyInit(void)
{
    rx_wr = 0U;
    rx_rd = 0U;
    rx_task_handle = NULL;
    uart4_tx_busy = 0U;
    HAL_UART_Receive_IT(&huart4, &rx_byte, 1);
}


/**
 * @brief Inizializza il modulo UART nel contesto del task chiamante.
 */
void CommUart_Init(void)
{
    CommUart_RegisterRxTask(xTaskGetCurrentTaskHandle());
}

/**
 * @brief Callback HAL di completamento trasmissione UART.
 * @param huart UART che ha completato la TX.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart4)
    {
        uart4_tx_busy = 0U;
    }
    else if (huart == &huart5)
    {
        SabertoothCallback();
    }
}


/**
 * @brief Invia un buffer su UART4 in modalita' interrupt.
 * @param buf Puntatore al buffer dati.
 * @param len Numero di byte da inviare.
 */
void CommUart_Send(const uint8_t *buf, uint16_t len)
{
    if (!buf || len == 0)
    {
        return;
    }

    if (uart4_tx_busy)
    {
        return;
    }

    uart4_tx_busy = 1U;
    if (HAL_UART_Transmit_IT(&huart4, (uint8_t *)buf, len) != HAL_OK)
    {
        uart4_tx_busy = 0U;
    }
}


/**
 * @brief Callback HAL di completamento ricezione UART.
 * @param huart UART che ha ricevuto il byte.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart4)
    {
        uint16_t next = (rx_wr + 1U) % UART_RX_BUF_SIZE;

        /* Scrive il byte solo se il buffer non e' pieno. */
        if (next != rx_rd)
        {
            rx_buf[rx_wr] = rx_byte;
            rx_wr = next;
        }

        BaseType_t hpw = pdFALSE;
        if (rx_task_handle)
        {
            vTaskNotifyGiveFromISR(rx_task_handle, &hpw);
        }

        HAL_UART_Receive_IT(&huart4, &rx_byte, 1);

        portYIELD_FROM_ISR(hpw);
    }
}

/**
 * @brief Legge un byte dal buffer RX se disponibile.
 * @param b Puntatore al byte di output.
 * @return `true` se e' stato letto un byte, `false` se il buffer e' vuoto.
 */
bool CommUart_GetByte(uint8_t *b)
{
    if (b == NULL)
    {
        return false;
    }

    if (rx_rd == rx_wr)
    {
        return false;
    }

    *b = rx_buf[rx_rd];
    rx_rd = (rx_rd + 1U) % UART_RX_BUF_SIZE;
    return true;
}

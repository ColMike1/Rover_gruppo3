/*
 * comm_uart.c
 *
 * Created on: Jan 11, 2026
 * Author: Sterm
 */

#include "comm/comm_uart.h"
#include "stm32g4xx_hal.h"
#include "actuation/sabertooth_driver.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>
#include <stdbool.h>

/* ===================== Configuration ===================== */

#define UART_RX_BUF_SIZE   (256U)

/* ===================== External handles ===================== */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

/* ===================== Static data ===================== */

static volatile bool uart4_tx_busy = false;

static uint8_t rx_byte;
static uint8_t rx_buf[UART_RX_BUF_SIZE];

static volatile uint16_t rx_wr = 0U;
static volatile uint16_t rx_rd = 0U;

static TaskHandle_t rx_task_handle = NULL;

/* ===================== Local utilities ===================== */

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

void CommUart_RegisterRxTask(TaskHandle_t h)
{
    rx_task_handle = h;
}

void CommUart_EarlyInit(void)
{
    rx_wr = 0U;
    rx_rd = 0U;
    rx_task_handle = NULL;
    uart4_tx_busy = false;

    (void)HAL_UART_Receive_IT(&huart4, &rx_byte, 1U);
}

void CommUart_Init(void)
{
    CommUart_RegisterRxTask(xTaskGetCurrentTaskHandle());
}

void CommUart_Send(const uint8_t *buf, uint16_t len)
{
    HAL_StatusTypeDef st;

    if ((buf != NULL) && (len > 0U))
    {
        st = HAL_UART_Transmit(&huart4, buf, len, 3U);
        (void)st;
    }
}

/* ===================== HAL callbacks ===================== */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        if (huart == &huart5)
        {
            SabertoothCallback();
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t hpw = pdFALSE;

    if (huart == &huart4)
    {
        uint16_t next = ring_next(rx_wr);

        if (next != rx_rd)
        {
            rx_buf[rx_wr] = rx_byte;
            rx_wr = next;
        }

        if (rx_task_handle != NULL)
        {
            vTaskNotifyGiveFromISR(rx_task_handle, &hpw);
        }

        (void)HAL_UART_Receive_IT(&huart4, &rx_byte, 1U);
        portYIELD_FROM_ISR(hpw);
    }
}

/* ===================== RX API ===================== */

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

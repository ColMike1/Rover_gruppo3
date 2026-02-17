#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32_hal.h"
#include "supervisor_snapshot.h"
#include "encoder_snapshot.h"

UART_HandleTypeDef huart5;
EncoderSnapshot_t mock_enc;

float last_applied_voltages[4];
uint8_t saber_tx[16];


extern volatile uint8_t tx_busy;

void Mock_Actuation_Reset(void) {
    tx_busy = 0;
    memset(saber_tx, 0, sizeof(saber_tx));
}

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size) {
    return HAL_OK;
}


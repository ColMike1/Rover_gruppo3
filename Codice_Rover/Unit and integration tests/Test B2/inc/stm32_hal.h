#ifndef STM32_HAL_H
#define STM32_HAL_H

#include <stdint.h>
#include "i2c.h"

typedef enum {
    HAL_OK = 0,
    HAL_ERROR
} HAL_StatusTypeDef;

HAL_StatusTypeDef HAL_I2C_Mem_Read(
    I2C_HandleTypeDef *hi2c,
    uint16_t devAddr,
    uint16_t memAddr,
    uint16_t memAddrSize,
    uint8_t *pData,
    uint16_t size,
    uint32_t timeout
);

HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef *hi2c,
    uint16_t devAddr,
    uint16_t memAddr,
    uint16_t memAddrSize,
    uint8_t *pData,
    uint16_t size,
    uint32_t timeout
);

typedef struct {
    uint32_t Instance; // Non ci serve davvero, ma evita errori
} UART_HandleTypeDef;

HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif

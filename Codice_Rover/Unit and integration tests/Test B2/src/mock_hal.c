#include "stm32_hal.h"
#include "gpio.h"
#include <string.h>

uint8_t mock_mpu_registers[14];
uint8_t last_uart_buffer[64];
uint16_t last_uart_size = 0;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
GPIO_PinState last_gpio_state = GPIO_PIN_RESET;

HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size) {
    last_uart_size = Size;
    uint16_t copy_size = (Size > 64) ? 64 : Size;
    memcpy(last_uart_buffer, pData, copy_size);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size) { return HAL_OK; }

HAL_StatusTypeDef HAL_I2C_Mem_Read(
    I2C_HandleTypeDef *hi2c,
    uint16_t devAddr,
    uint16_t memAddr,
    uint16_t memAddrSize,
    uint8_t *pData,
    uint16_t size,
    uint32_t timeout
)
{
	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef *hi2c,
    uint16_t devAddr,
    uint16_t memAddr,
    uint16_t memAddrSize,
    uint8_t *pData,
    uint16_t size,
    uint32_t timeout
)
{
    return HAL_OK;
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    last_gpio_state = PinState;
}

#ifndef STM32_HAL_H
#define STM32_HAL_H

#include <stdint.h>
#include <stddef.h>

#define HAL_MAX_DELAY      0xFFFFFFFFU

typedef uint32_t HAL_TickFreqTypeDef;

typedef enum {
    HAL_OK       = 0x00,
    HAL_ERROR    = 0x01,
    HAL_BUSY     = 0x02,
    HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

typedef struct {
    uint32_t Instance; // Non usata nei mock, ma necessaria per la compilazione
} UART_HandleTypeDef;

typedef struct {
    uint32_t Instance;
} ADC_HandleTypeDef;


/**
 * @brief Trasmissione UART in modalità Blocking (usata in Sabertooth_Init)
 */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart,
                                    uint8_t *pData,
                                    uint16_t Size,
                                    uint32_t Timeout);

/**
 * @brief Trasmissione UART in modalità Interrupt (usata in Sabertooth_ApplyOutputs)
 */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart,
                                       uint8_t *pData,
                                       uint16_t Size);


HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);


#endif /* STM32_HAL_H */

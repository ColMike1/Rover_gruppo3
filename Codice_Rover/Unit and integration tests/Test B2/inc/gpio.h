#ifndef MOCK_GPIO_H
#define MOCK_GPIO_H

#include <stdint.h>

#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET   1
typedef uint8_t GPIO_PinState;

typedef struct {
    uint32_t dummy;
} GPIO_TypeDef;

#define RED_LED_GPIO_Port            ((GPIO_TypeDef *) 0x1)
#define YELLOW_LED_LEFT_GPIO_Port    ((GPIO_TypeDef *) 0x2)
#define YELLOW_LED_RIGHT_GPIO_Port   ((GPIO_TypeDef *) 0x3)
#define BLUE_LED_GPIO_Port           ((GPIO_TypeDef *) 0x4)
#define ESTOP_GPIO_Port      		 ((GPIO_TypeDef *) 0x10)
#define RELAY_IN_GPIO_Port   		 ((GPIO_TypeDef *) 0x11)

#define RED_LED_Pin            (1 << 0)
#define YELLOW_LED_LEFT_Pin    (1 << 1)
#define YELLOW_LED_RIGHT_Pin   (1 << 2)
#define BLUE_LED_Pin           (1 << 3)
#define ESTOP_Pin              (1 << 4)
#define RELAY_IN_Pin           (1 << 5)

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif

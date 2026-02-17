#include "gpio.h"

GPIO_PinState estop_state = GPIO_PIN_RESET;
GPIO_PinState relay_state = GPIO_PIN_RESET;
extern GPIO_PinState mock_red_led;
extern GPIO_PinState mock_yellow_l_led;
extern GPIO_PinState mock_yellow_r_led;
extern GPIO_PinState mock_blue_led;

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    if (GPIO_Pin == ESTOP_Pin) {
        estop_state = PinState;
    }
    if (GPIO_Pin == RELAY_IN_Pin) {
        relay_state = PinState;
    }
    if (GPIO_Pin == RED_LED_Pin) mock_red_led = PinState;
    if (GPIO_Pin == YELLOW_LED_LEFT_Pin) mock_yellow_l_led = PinState;
    if (GPIO_Pin == YELLOW_LED_RIGHT_Pin) mock_yellow_r_led = PinState;
    if (GPIO_Pin == BLUE_LED_Pin) mock_blue_led = PinState;
}

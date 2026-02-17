#include "gpio.h"
#include "supervisor_snapshot.h"
#include "rx_snapshot.h"
#include <string.h>

GPIO_PinState mock_red_led = GPIO_PIN_RESET;
GPIO_PinState mock_yellow_l_led = GPIO_PIN_RESET;
GPIO_PinState mock_yellow_r_led = GPIO_PIN_RESET;
GPIO_PinState mock_blue_led = GPIO_PIN_RESET;

extern int n;
extern SupervisorSnapshot_t mock_sup_data;
extern RxSnapshot_t         mock_rx;

void Mock_Led_Reset(void) {
    mock_red_led = GPIO_PIN_RESET;
    mock_yellow_l_led = GPIO_PIN_RESET;
    mock_yellow_r_led = GPIO_PIN_RESET;
    mock_blue_led = GPIO_PIN_RESET;
    memset(&mock_sup_data, 0, sizeof(mock_sup_data));
    memset(&mock_rx, 0, sizeof(mock_rx));
    n = 0;
}

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    if (GPIO_Pin == RED_LED_Pin) {
        mock_red_led = (mock_red_led == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
}

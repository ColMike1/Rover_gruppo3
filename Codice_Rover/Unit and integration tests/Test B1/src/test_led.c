#include "unity.h"
#include "led_task.h"
#include "supervisor_snapshot.h"
#include "rx_snapshot.h"
#include "gpio.h"

extern GPIO_PinState mock_red_led;
extern GPIO_PinState mock_yellow_l_led;
extern GPIO_PinState mock_yellow_r_led;
extern GPIO_PinState mock_blue_led;
extern SupervisorSnapshot_t mock_sup_data;
extern RxSnapshot_t mock_rx;
extern void Mock_Led_Reset();

void setUp_led(void) {
    Mock_Led_Reset();
}

void test_Led_RedSolidOnCritical(void) {
	setUp_led();
    mock_sup_data.critical_mask = 0x01;
    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_red_led);

    Mock_Led_Reset();
    mock_rx.payload.critical_mask = 0x02;
    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_red_led);
}

void test_Led_RedBlinksOnDegraded(void) {
	setUp_led();
    mock_sup_data.degraded_mask = 0x01;

    for(int i = 0; i < 5; i++) {
        Led_TaskStep();
    }
    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, mock_red_led);

    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_red_led);

    for(int i = 0; i <5; i++) {
        Led_TaskStep();
    }
    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, mock_red_led);
}

void test_Led_YellowDirectionIndicators(void) {
	setUp_led();

    mock_sup_data.current_action = CMD_GO_LEFT;
    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_yellow_l_led);
    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, mock_yellow_r_led);

    mock_sup_data.current_action = CMD_GO_RIGHT;
    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, mock_yellow_l_led);
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_yellow_r_led);
}

void test_Led_BlueOnEStop(void) {
	setUp_led();
    mock_sup_data.current_action = CMD_ESTOP;
    Led_TaskStep();
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_blue_led);
}

void test_Led_CriticalOverridesDegraded(void) {
	setUp_led();
    mock_sup_data.degraded_mask = 0x01;
    mock_sup_data.critical_mask = 0x01;

    Led_TaskStep();

    TEST_ASSERT_EQUAL(GPIO_PIN_SET, mock_red_led);
}

void test_led(void){
	RUN_TEST(test_Led_RedSolidOnCritical);
	RUN_TEST(test_Led_RedBlinksOnDegraded);
	RUN_TEST(test_Led_YellowDirectionIndicators);
	RUN_TEST(test_Led_BlueOnEStop);
	RUN_TEST(test_Led_CriticalOverridesDegraded);
}

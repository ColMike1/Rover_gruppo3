#include "unity.h"
#include "actuation.h"
#include <string.h>

extern uint8_t last_uart_buffer[64];
extern float u_left_rpm_last;
extern float u_right_rpm_last;

void test_Actuation_Step_Saturation(void) {
    SabertoothCallback();

    for(int i = 0; i < 25; i++){
    	SabertoothCallback();
    	Actuation_Step(1.0f, 0.0f);
    }

    TEST_ASSERT_INT_WITHIN(2, 64, last_uart_buffer[2]);
}

void test_Actuation_Step_RateLimiter(void) {
	u_left_rpm_last = 0.0f;
	u_right_rpm_last = 0.0f;
    SabertoothCallback();

    Actuation_Step(1.0f, 0.0f);

    TEST_ASSERT_INT_WITHIN(1, 3, last_uart_buffer[2]);

    SabertoothCallback();

    Actuation_Step(1.0f, 0.0f);
    TEST_ASSERT_INT_WITHIN(1, 6, last_uart_buffer[2]);
}

void test_Actuation_Step_Turning(void) {
    SabertoothCallback();

    for(int i = 0; i < 5; i++){
    	SabertoothCallback();
    	Actuation_Step(0.0f, 0.1f);
    }

    TEST_ASSERT_EQUAL(1, last_uart_buffer[1]);

    TEST_ASSERT_EQUAL(4, last_uart_buffer[5]);
}

void test_actuation(void){
	RUN_TEST(test_Actuation_Step_Saturation);
	RUN_TEST(test_Actuation_Step_RateLimiter);
	RUN_TEST(test_Actuation_Step_Turning);
}

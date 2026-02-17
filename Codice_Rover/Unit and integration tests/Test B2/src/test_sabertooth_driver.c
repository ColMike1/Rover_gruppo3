#include "unity.h"
#include "sabertooth_driver.h"
#include <string.h>

extern uint8_t last_uart_buffer[64];
extern uint16_t last_uart_size;

void setUp_saber(void) {
    memset(last_uart_buffer, 0, sizeof(last_uart_buffer));
    last_uart_size = 0;
    SabertoothCallback();
}

void test_Sabertooth_Init_SendsCorrectPackets(void) {
	setUp_saber();
    Sabertooth_Init();

    TEST_ASSERT_EQUAL(8, last_uart_size);

    TEST_ASSERT_EQUAL_UINT8(134, last_uart_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(14,  last_uart_buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(3,   last_uart_buffer[2]);

    TEST_ASSERT_EQUAL_UINT8(23,  last_uart_buffer[3]);

    TEST_ASSERT_EQUAL_UINT8(135, last_uart_buffer[4]);
    TEST_ASSERT_EQUAL_UINT8(14,  last_uart_buffer[5]);
    TEST_ASSERT_EQUAL_UINT8(3,   last_uart_buffer[6]);

    TEST_ASSERT_EQUAL_UINT8(24,  last_uart_buffer[7]);
}

void test_Sabertooth_ApplyOutputs_ForwardFullSpeed(void) {
	setUp_saber();

    Sabertooth_ApplyOutputs(12.0f, 12.0f, 12.0f, 12.0f);


    TEST_ASSERT_EQUAL(0, last_uart_buffer[1]);
    TEST_ASSERT_EQUAL(127, last_uart_buffer[2]);
}

void test_Sabertooth_ApplyOutputs_Reverse(void) {
	setUp_saber();

    Sabertooth_ApplyOutputs(-6.0f, 0, 0, 0);

    TEST_ASSERT_EQUAL(1, last_uart_buffer[1]);
    TEST_ASSERT_INT_WITHIN(1, 64, last_uart_buffer[2]);
}

void test_sabertooth(void){
	RUN_TEST(test_Sabertooth_Init_SendsCorrectPackets);
	RUN_TEST(test_Sabertooth_ApplyOutputs_ForwardFullSpeed);
	RUN_TEST(test_Sabertooth_ApplyOutputs_Reverse);
}

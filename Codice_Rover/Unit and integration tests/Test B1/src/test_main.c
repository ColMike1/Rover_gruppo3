#include "unity.h"

extern void test_actuation();
extern void test_board_health();
extern void test_comm();
extern void test_control();
extern void test_encoder();
extern void test_led();
extern void test_supervisor();

void setUp(void) {}

void tearDown(void) {}


int main(void)
{
    printf("\n--- AVVIO UNIT TESTS ---\n");

    UNITY_BEGIN();

    test_actuation();
    test_board_health();
    test_comm();
    test_control();
    test_encoder();
    test_led();
    test_supervisor();

    printf("\n--- TEST COMPLETATI ---\n");

    return UNITY_END();
}

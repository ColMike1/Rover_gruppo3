#include "unity.h"

extern void test_imu();
extern void test_ble();
extern void test_sonar();
extern void test_comm();
extern void test_actuation();
extern void test_sabertooth();
extern void test_supervisor();

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    printf("\n--- AVVIO UNIT TESTS ---\n");

    UNITY_BEGIN();

    test_imu();
    test_ble();
    test_sonar();
    test_comm();
    test_actuation();
    test_sabertooth();
    test_supervisor();

    printf("\n--- TEST COMPLETATI ---\n");

    return UNITY_END();
}

#include "unity.h"
#include "sabertooth_driver.h"
#include "actuation.h"
#include "encoder_snapshot.h"
#include <string.h>

extern uint8_t saber_tx[16];
extern EncoderSnapshot_t mock_enc;
extern float last_applied_voltages[4];
extern void Mock_Actuation_Reset();

void setUp_actuation(void) {
    memset(saber_tx, 0, sizeof(saber_tx));
    memset(&mock_enc, 0, sizeof(EncoderSnapshot_t));
}



void test_Sabertooth_Packet_Encoding(void) {

	Mock_Actuation_Reset();
    Sabertooth_ApplyOutputs(0, 0, 12.0f, 0);

    TEST_ASSERT_EQUAL_UINT8(SABER_BACK_ADDR, saber_tx[0]);
    TEST_ASSERT_EQUAL_UINT8(0,   saber_tx[1]);
    TEST_ASSERT_EQUAL_UINT8(127, saber_tx[2]);
    TEST_ASSERT_EQUAL_UINT8(5, saber_tx[3]);
}


void test_Sabertooth_Direction_Logic(void) {
	Mock_Actuation_Reset();
    Sabertooth_ApplyOutputs(0, 0, -6.0f, 0);

    TEST_ASSERT_EQUAL_UINT8(1,  saber_tx[1]);
    TEST_ASSERT_EQUAL_UINT8(64, saber_tx[2]);
}

void test_Actuation_Voltage_Saturation(void) {
    ControlOutput_t cmd = { .u_sx_a = 15.0f, .u_dx_a = -50.0f };

    Actuation_Step(cmd);
}

void test_Actuation_Safety_Deadzone(void) {

    ControlOutput_t cmd = { .u_sx_a = 0.4f };


    mock_enc.hasNoFeedback[0] = false;
    Actuation_Step(cmd);

    mock_enc.hasNoFeedback[0] = true;
    Actuation_Step(cmd);
}

void test_actuation(void){

	RUN_TEST(test_Sabertooth_Packet_Encoding);
	RUN_TEST(test_Sabertooth_Direction_Logic);
	RUN_TEST(test_Actuation_Voltage_Saturation);
	RUN_TEST(test_Actuation_Safety_Deadzone);
}

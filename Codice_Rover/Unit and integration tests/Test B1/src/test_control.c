#include "unity.h"
#include "control_law.h"
#include "control_step.h"
#include "encoder_snapshot.h"
#include "supervisor_snapshot.h"
#include "rx_snapshot.h"
#include <math.h>
#include <string.h>

extern EncoderSnapshot_t mock_enc_data;
extern SupervisorSnapshot_t mock_sup_data;
extern RxSnapshot_t         mock_rx;

void setUp_control(void) {
    memset(&mock_enc_data, 0, sizeof(mock_enc_data));
    memset(&mock_sup_data, 0, sizeof(mock_sup_data));
    memset(&mock_rx, 0, sizeof(mock_rx));
    Control_Init();
}

void test_Control_ReferenceScaling_ShouldLimitTotalSpeed(void) {
	setUp_control();
    mock_sup_data.speed_ref_rpm = 1.0f;
    mock_sup_data.steering_cmd = 1.0f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_TRUE(out.u_dx_a != 0.0f);
}

void test_Control_Fallback_OneEncoderFault(void) {
	setUp_control();
	mock_sup_data.degraded_mask = FAULT_WHEEL_FL;
    mock_sup_data.speed_ref_rpm = 0.5f;

    for(int i=0; i<4; i++) mock_enc_data.wheel_speed_rpm[i] = 80.0f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_EQUAL_FLOAT(out.u_sx_p, out.u_sx_a);
}

void test_Control_OpenLoop_WhenAllEncodersFail(void) {
	setUp_control();

    mock_sup_data.degraded_mask = (FAULT_WHEEL_FL | FAULT_WHEEL_FR | FAULT_WHEEL_RL | FAULT_WHEEL_RR);
    mock_sup_data.speed_ref_rpm = 0.5f;
    mock_sup_data.steering_cmd = 0.0f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_EQUAL_FLOAT(6.0f, out.u_sx_a);
    TEST_ASSERT_EQUAL_FLOAT(6.0f, out.u_dx_a);
}

void test_PI_Compute_ShouldIncreaseOutputOnError(void) {
	setUp_control();

    mock_sup_data.speed_ref_rpm = 0.5f;
    for(int i=0; i<4; i++) mock_enc_data.wheel_speed_rpm[i] = 0.0f;

    ControlOutput_t out1 = Control_Step();
    ControlOutput_t out2 = Control_Step();

    TEST_ASSERT_TRUE(out2.u_sx_a > out1.u_sx_a);
}

void test_PI_Compute_ShouldSaturate(void) {
	setUp_control();

    mock_sup_data.speed_ref_rpm = 1.0f;
    for(int i=0; i<4; i++) mock_enc_data.wheel_speed_rpm[i] = -500.0f;

    ControlOutput_t out;
    for(int i=0; i<20; i++) out = Control_Step();

    TEST_ASSERT_TRUE(fabsf(out.u_sx_a) <= 25.0f);
}

void test_Control_Fallback_CrossedFaults_FL_RR(void) {
	setUp_control();

    mock_sup_data.degraded_mask = (FAULT_WHEEL_FL | FAULT_WHEEL_RR);
    mock_sup_data.speed_ref_rpm = 0.5f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_EQUAL_FLOAT(out.u_sx_p, out.u_sx_a);
    TEST_ASSERT_EQUAL_FLOAT(out.u_dx_a, out.u_dx_p);
}

void test_Control_Fallback_ThreeFailures_OnlyFR_Alive(void) {
	setUp_control();

    mock_sup_data.degraded_mask = (FAULT_WHEEL_FL | FAULT_WHEEL_RL | FAULT_WHEEL_RR);

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_EQUAL_FLOAT(out.u_dx_a, out.u_sx_a);
    TEST_ASSERT_EQUAL_FLOAT(out.u_dx_a, out.u_sx_p);
    TEST_ASSERT_EQUAL_FLOAT(out.u_dx_a, out.u_dx_p);
}

void test_Control_DisableSteering_OnLateralEncoderFailure(void) {
	setUp_control();

    mock_sup_data.degraded_mask = (FAULT_WHEEL_FL | FAULT_WHEEL_FR);
    mock_sup_data.speed_ref_rpm = 0.5f;
    mock_sup_data.steering_cmd = 0.8f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_FLOAT_WITHIN(1.0f, out.u_sx_p, out.u_dx_p);
}

void test_ControlLaw_AsseSynchronization(void) {
	setUp_control();

    mock_sup_data.speed_ref_rpm = 0.5f;
    mock_enc_data.wheel_speed_rpm[0] = 100.0f;
    mock_enc_data.wheel_speed_rpm[1] = 100.0f;
    mock_enc_data.wheel_speed_rpm[2] = 60.0f;
    mock_enc_data.wheel_speed_rpm[3] = 60.0f;

    ControlOutput_t out = Control_Step();

    TEST_ASSERT_TRUE(out.u_sx_p > out.u_sx_a);
}

void test_control(void){
	RUN_TEST(test_Control_ReferenceScaling_ShouldLimitTotalSpeed);
	RUN_TEST(test_Control_Fallback_OneEncoderFault);
	RUN_TEST(test_Control_OpenLoop_WhenAllEncodersFail);
	RUN_TEST(test_PI_Compute_ShouldIncreaseOutputOnError);
	RUN_TEST(test_Control_Fallback_CrossedFaults_FL_RR);
	RUN_TEST(test_Control_Fallback_ThreeFailures_OnlyFR_Alive);
	RUN_TEST(test_Control_DisableSteering_OnLateralEncoderFailure);
	RUN_TEST(test_ControlLaw_AsseSynchronization);
	RUN_TEST(test_PI_Compute_ShouldSaturate);
}

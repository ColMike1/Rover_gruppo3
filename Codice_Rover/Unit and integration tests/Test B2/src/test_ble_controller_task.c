#include "unity.h"
#include "ble_controller_task.h"
#include "ble_controller_i2c.h"
#include "ble_controller_snapshot.h"
#include <string.h>

extern BleI2CStatus_t mock_ble_status;
extern BleRawFrame_t mock_ble_frame;
extern BleControllerSnapshot_t captured_snap;
extern int snapshot_write_count;
extern uint32_t fake_tick;

void setUp_ble(void) {
    memset(&mock_ble_frame, 0, sizeof(BleRawFrame_t));
    memset(&captured_snap, 0, sizeof(BleControllerSnapshot_t));
    snapshot_write_count = 0;
    mock_ble_status = BLE_I2C_COMPLETE;
    fake_tick = 1000;
}

void test_NormalizeAxis_Center(void) {
	setUp_ble();
    mock_ble_frame.ax = 255;
    mock_ble_frame.ay = 255;

    BleController_TaskStep();

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, captured_snap.ax_norm);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, captured_snap.ay_norm);
}

void test_NormalizeAxis_Deadzone(void) {
	setUp_ble();

    mock_ble_frame.ax = 260;

    BleController_TaskStep();

    TEST_ASSERT_EQUAL_FLOAT(0.0f, captured_snap.ax_norm);
}

void test_NormalizeAxis_Saturation(void) {
	setUp_ble();
    mock_ble_frame.ax = 600;
    mock_ble_frame.ay = 0;

    BleController_TaskStep();

    TEST_ASSERT_EQUAL_FLOAT(1.0f, captured_snap.ax_norm);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, captured_snap.ay_norm);
}

void test_TaskStep_I2C_Error(void) {
	setUp_ble();
    mock_ble_status = BLE_I2C_ERROR;
    fake_tick = 2000;

    captured_snap.data_last_valid_ms = 1000;

    BleController_TaskStep();

    TEST_ASSERT_EQUAL_UINT32(1000, captured_snap.data_last_valid_ms);

    TEST_ASSERT_EQUAL_UINT32(2000, captured_snap.task_last_run_ms);
}

void test_ble(void){

    RUN_TEST(test_NormalizeAxis_Center);
    RUN_TEST(test_NormalizeAxis_Deadzone);
    RUN_TEST(test_NormalizeAxis_Saturation);
    RUN_TEST(test_TaskStep_I2C_Error);
}

#include "ble_controller_i2c.h"
#include "ble_controller_snapshot.h"
#include "cmsis_os2.h"
#include <string.h>

BleI2CStatus_t mock_ble_status = BLE_I2C_COMPLETE;
BleRawFrame_t mock_ble_frame;
BleControllerSnapshot_t captured_snap;
int snapshot_write_count = 0;

BleI2CStatus_t BleController_I2C_ReadFrame(BleRawFrame_t *frame) {
    if (mock_ble_status == BLE_I2C_COMPLETE) {
        *frame = mock_ble_frame;
    }
    return mock_ble_status;
}

void BleControllerSnapshot_Write(BleControllerSnapshot_t *snap) {
    captured_snap = *snap;
    snapshot_write_count++;
}

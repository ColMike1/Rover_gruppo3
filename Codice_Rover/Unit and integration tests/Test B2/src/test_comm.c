#include "unity.h"
#include <string.h>
#include "comm_pack.h"
#include "comm_unpack.h"
#include "comm_uart.h"
#include "comm_rx_task.h"
#include "rx_snapshot.h"

extern RxSnapshot_t captured_rx_snap;
extern void Mock_UART_InjectByte(uint8_t b);
extern void Mock_UART_Clear(void);

void setUp_comm(void) {
    Mock_UART_Clear();
    memset(&captured_rx_snap, 0, sizeof(RxSnapshot_t));
}

void test_CommPack_BuildB2_Success(void) {
    setUp_comm();
    uint8_t buffer[128];
    BleControllerSnapshot_t ble = { .bx_norm = 0.5f, .ay_norm = -0.5f };
    SupervisorSnapshot_t sup = { .task_last_run_ms = 5000, .command = 0x01 };
    IMUSnapshot_t imu = { .yaw = 180.0f };

    uint16_t len = CommPack_BuildB2Tx(buffer, sizeof(buffer), &ble, &sup, &imu);

    CommFrameHeader_t *hdr = (CommFrameHeader_t *)buffer;
    TEST_ASSERT_EQUAL_UINT16(sizeof(CommFrameB2_t), len);
    TEST_ASSERT_EQUAL_HEX16(0xAA56, hdr->msg_id);
}

void test_CommUnpack_B1_CRC_Error(void) {
    setUp_comm();
    uint8_t raw_frame[sizeof(CommFrameB1_t)];
    memset(raw_frame, 0xA5, sizeof(raw_frame));

    CommFrameHeader_t hdr;
    CommPayloadB1_t pl;

    CommUnpackStatus_t st = CommUnpack_B2FromB1(raw_frame, sizeof(raw_frame), &hdr, &pl);

    TEST_ASSERT_EQUAL(COMM_UNPACK_CRC, st);
}

void test_RxTask_Sync_Recovery(void) {
    setUp_comm();

    Mock_UART_InjectByte(0xFF);
    Mock_UART_InjectByte(0x55);
    Mock_UART_InjectByte(0x55);
    Mock_UART_InjectByte(0xAA);

    Rx_TaskStep();

    TEST_ASSERT_NOT_EQUAL(COMM_UNPACK_NULL, captured_rx_snap.last_event);
}

void test_comm(void){
	RUN_TEST(test_CommPack_BuildB2_Success);
	RUN_TEST(test_CommUnpack_B1_CRC_Error);
	RUN_TEST(test_RxTask_Sync_Recovery);
}

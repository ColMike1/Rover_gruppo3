#include "unity.h"
#include "comm_pack.h"
#include "comm_unpack.h"
#include <string.h>

uint8_t buffer[256];

void setUp_comm(void) {
    memset(buffer, 0, sizeof(buffer));
}

void test_CommPack_BuildB1_ShouldFillBufferCorrectmente(void) {
    setUp_comm();
    EncoderSnapshot_t enc = {0};
    SupervisorSnapshot_t sup = {0};

    enc.wheel_speed_rpm[0] = 1500;
    sup.alive_counter = 10;
    sup.task_last_run_ms = 5000;

    uint16_t len = CommPack_BuildB1Tx(buffer, sizeof(buffer), &enc, &sup);

    TEST_ASSERT_EQUAL_UINT16(sizeof(CommFrameB1_t), len);

    TEST_ASSERT_EQUAL_UINT8(0x55, buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0xAA, buffer[1]);
}

void test_CommUnpack_ShouldReturnOkOnValidFrame(void) {
    setUp_comm();
    CommFrameHeader_t hdr_out;
    CommPayloadB2_t pl_out;

    uint8_t test_buf[sizeof(CommFrameB2_t)];
    memset(test_buf, 0, sizeof(test_buf));

    uint16_t crc = crc16_ccitt(test_buf, sizeof(CommFrameB2_t) - 2);
    memcpy(&test_buf[sizeof(CommFrameB2_t) - 2], &crc, 2);

    CommUnpackStatus_t status = CommUnpack_B1FromB2(test_buf, sizeof(test_buf), &hdr_out, &pl_out);

    TEST_ASSERT_EQUAL(COMM_UNPACK_OK, status);
}

void test_CommUnpack_ShouldDetectCrcError(void) {
    setUp_comm();
    uint8_t test_buf[sizeof(CommFrameB2_t)];
    memset(test_buf, 0xA5, sizeof(test_buf));

    CommFrameHeader_t hdr;
    CommPayloadB2_t pl;

    CommUnpackStatus_t status = CommUnpack_B1FromB2(test_buf, sizeof(test_buf), &hdr, &pl);

    TEST_ASSERT_EQUAL(COMM_UNPACK_CRC, status);
}

void test_Rx_Sync_Logic(void) {
    setUp_comm();
    uint8_t acc_buf[64];
    uint16_t acc_len = 0;
    uint16_t SYNC = 0xAA56;

    // Byte di spazzatura
    uint8_t stream[] = { 0xFF, 0x00, 0x56, 0xAA, 0x12, 0x34 };
    // Nota: 0x56 0xAA è 0xAA56 in little-endian

    // Logica di Sync semplificata dal tuo codice
    for(int i=0; i<6; i++) {
        uint8_t byte = stream[i];
        if (acc_len < 2) {
            acc_buf[acc_len++] = byte;
            if (acc_len == 2) {
                uint16_t sync = acc_buf[0] | (acc_buf[1] << 8);
                if (sync != SYNC) {
                    acc_buf[0] = acc_buf[1];
                    acc_len = 1;
                }
            }
        }
    }

    // Alla fine dello stream "FF 00 56 AA", acc_len deve essere 2 e contenere la SYNC
    TEST_ASSERT_EQUAL_UINT16(2, acc_len);
    uint16_t final_sync = acc_buf[0] | (acc_buf[1] << 8);
    TEST_ASSERT_EQUAL_UINT16(SYNC, final_sync);
}

void test_comm(void){
	RUN_TEST(test_CommPack_BuildB1_ShouldFillBufferCorrectmente);
	RUN_TEST(test_CommUnpack_ShouldReturnOkOnValidFrame);
	RUN_TEST(test_CommUnpack_ShouldDetectCrcError);
	RUN_TEST(test_Rx_Sync_Logic);
}

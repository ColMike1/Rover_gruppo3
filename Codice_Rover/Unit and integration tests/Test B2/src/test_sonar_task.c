#include "unity.h"
#include "sonar_task.h"
#include "sonar_hw.h"
#include "sonar_snapshot.h"
#include "tim.h"
#include <string.h>
#include <stdbool.h>

extern buffer_sonars_t buffers;
extern sonars_flag_t flag;
extern SonarSnapshot_t captured_sonar_snap;
extern bool simulate_timeout;
extern uint8_t sonar_count;

void setUp_sonar(void) {
    memset(&buffers, 0, sizeof(buffers));
    memset(&flag, 0, sizeof(flag));
    simulate_timeout = false;
}

void test_ReadDistance_Calculation(void) {
    setUp_sonar();
    uint16_t test_buf[2] = {1000, 1580};
    uint32_t result = read_distance(test_buf);

    TEST_ASSERT_EQUAL_UINT32(10, result);
}

void test_ReadDistance_TimerOverflow(void) {
    setUp_sonar();
    uint16_t test_buf[2] = {65500, 58};
    uint32_t result = read_distance(test_buf);

    TEST_ASSERT_EQUAL_UINT32(1, result);
}

void test_SonarTaskStep_Success(void) {
    setUp_sonar();
    buffers.buf_ch1[0] = 0; buffers.buf_ch1[1] = 580;
    buffers.buf_ch2[0] = 0; buffers.buf_ch2[1] = 1160;
    buffers.buf_ch3[0] = 0; buffers.buf_ch3[1] = 1740;

    flag.sonar1_ok = 1;
    flag.sonar2_ok = 1;
    flag.sonar3_ok = 1;

    Sonar_TaskStep();

    TEST_ASSERT_EQUAL_UINT16(10, captured_sonar_snap.dist_cm[0]);
    TEST_ASSERT_EQUAL_UINT16(20, captured_sonar_snap.dist_cm[1]);
    TEST_ASSERT_EQUAL_UINT16(30, captured_sonar_snap.dist_cm[2]);
}

void test_SonarTaskStep_Timeout(void) {
    setUp_sonar();
    simulate_timeout = true;
    flag.sonar1_ok = 0;
    flag.sonar2_ok = 1;

    buffers.buf_ch2[0] = 0; buffers.buf_ch2[1] = 580;

    Sonar_TaskStep();

    TEST_ASSERT_EQUAL_UINT16(301, captured_sonar_snap.dist_cm[0]);
}

void test_ISR_CaptureSequence(void) {
    setUp_sonar();
    TIM_HandleTypeDef htim;
    htim.Instance = TIM1;
    sonar_count = 0;

    htim.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    HAL_TIM_IC_CaptureCallback(&htim);

    TEST_ASSERT_EQUAL_UINT8(1, flag.sonar1_ok);
    TEST_ASSERT_EQUAL_UINT8(1, sonar_count);

    htim.Channel = HAL_TIM_ACTIVE_CHANNEL_3;
    HAL_TIM_IC_CaptureCallback(&htim);

    TEST_ASSERT_EQUAL_UINT8(1, flag.sonar3_ok);
    TEST_ASSERT_EQUAL_UINT8(2, sonar_count);
}

void test_sonar(void){

	RUN_TEST(test_ReadDistance_Calculation);
	RUN_TEST(test_ReadDistance_TimerOverflow);
	RUN_TEST(test_SonarTaskStep_Success);
	RUN_TEST(test_SonarTaskStep_Timeout);
	RUN_TEST(test_ISR_CaptureSequence);
}

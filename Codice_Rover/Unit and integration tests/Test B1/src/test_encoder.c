#include "unity.h"
#include "encoder.h"
#include "supervisor_snapshot.h"
#include "encoder_snapshot.h"
#include <string.h>

extern uint32_t mock_tick;
extern uint32_t fake_timer_counter[4];
extern SupervisorSnapshot_t mock_sup_data;
extern EncoderSnapshot_t mock_enc_data;

void setUp_encoder(void)
{
    mock_tick = 1000;

    for (int i = 0; i < 4; i++){
        fake_timer_counter[i] = 0;
    }

    mock_sup_data.degraded_mask = 0;
    mock_sup_data.critical_mask = 0;
    memset(&mock_enc_data, 0, sizeof(EncoderSnapshot_t));
}

void test_TimerOverflowHandledCorrectly(void)
{
	setUp_encoder();
    float cmd[4] = {5,5,5,5};

    fake_timer_counter[0] = 65530;
    Encoder_Step(cmd);

    fake_timer_counter[0] = 10;
    mock_tick += 10;

    Encoder_Step(cmd);

    TEST_ASSERT_TRUE(mock_enc_data.wheel_speed_rpm[0] > 0);
}

void test_PhysicalOverflowTriggersFault(void)
{
	setUp_encoder();
    float cmd[4] = {5,5,5,5};

    Encoder_Step(cmd);

    fake_timer_counter[0] = 10000;

    Encoder_Step(cmd);

    TEST_ASSERT_TRUE(mock_enc_data.hasNoFeedback[0]);
}

void test_StallDetectionIntegration(void)
{
	setUp_encoder();
    float cmd[4] = {5,0,0,0};

    for (int i = 0; i < 25; i++)
    {
        fake_timer_counter[0] += 0;
        mock_tick += 10;
        Encoder_Step(cmd);
    }

    TEST_ASSERT_TRUE(mock_enc_data.hasNoFeedback[0]);
}

void test_FallbackSingleWheel(void)
{
	setUp_encoder();
    float cmd[4] = {5,5,5,5};

    Encoder_Step(cmd);

    fake_timer_counter[0] = 100;
    fake_timer_counter[1] = 200;
    fake_timer_counter[2] = 300;
    fake_timer_counter[3] = 400;

    mock_sup_data.degraded_mask = FAULT_WHEEL_FL;

    Encoder_Step(cmd);

    TEST_ASSERT_EQUAL(
    		mock_enc_data.wheel_speed_rpm[2],
			mock_enc_data.wheel_speed_rpm[0]
    );
}

void test_encoder(void){
	RUN_TEST(test_TimerOverflowHandledCorrectly);
	RUN_TEST(test_PhysicalOverflowTriggersFault);
	RUN_TEST(test_StallDetectionIntegration);
	RUN_TEST(test_FallbackSingleWheel);
}

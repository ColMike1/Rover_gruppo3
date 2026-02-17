#include "unity.h"
#include "board_health_adc.h"
#include "board_health.h"
#include "board_health_snapshot.h"

extern uint16_t adc_raw[3];
extern volatile uint8_t adc_ready;
extern BoardHealthSnapshot_t last_snapshot_written;
extern uint32_t mock_tick;
extern uint16_t vrefint_cal;
extern uint16_t tcal1;
extern uint16_t tcal2;
#define ADC_RAW_VREF   0U
#define ADC_RAW_TEMP   1U
#define ADC_RAW_BATT   2U

void setUp_board(void)
{
    vrefint_cal = 1500U;
    tcal1 = 1000U;
    tcal2 = 1500U;
    adc_raw[ADC_RAW_VREF] = 1500U;
    mock_tick = 1000U;
}

void test_Temperature_OK(void)
{
	setUp_board();
    float temp = 0.0f;

    adc_raw[ADC_RAW_TEMP] = 1200U;

    BoardHealthStatus_t st = BoardHealth_ReadTemperature(&temp);

    TEST_ASSERT_EQUAL(BOARD_HEALTH_OK, st);
    TEST_ASSERT_TRUE(temp > -40.0f);
    TEST_ASSERT_TRUE(temp < 125.0f);
}

void test_Temperature_NullPointer(void)
{
	setUp_board();
    BoardHealthStatus_t st = BoardHealth_ReadTemperature(NULL);
    TEST_ASSERT_EQUAL(BOARD_HEALTH_ADC_ERROR, st);
}

void test_Temperature_VrefZero(void)
{
	setUp_board();
    float temp;
    adc_raw[ADC_RAW_VREF] = 0U;

    BoardHealthStatus_t st = BoardHealth_ReadTemperature(&temp);
    TEST_ASSERT_EQUAL(BOARD_HEALTH_ADC_ERROR, st);
}

void test_Battery_OK(void)
{
	setUp_board();
    float batt = 0.0f;

    adc_raw[ADC_RAW_BATT] = 2500U;

    BoardHealthStatus_t st = BoardHealth_ReadBattery(&batt);

    TEST_ASSERT_EQUAL(BOARD_HEALTH_OK, st);
    TEST_ASSERT_TRUE(batt >= 0.0f);
    TEST_ASSERT_TRUE(batt <= 100.0f);
}

void test_Battery_OutOfRange(void)
{
	setUp_board();
    float batt;

    adc_raw[ADC_RAW_BATT] = 10U;

    BoardHealthStatus_t st = BoardHealth_ReadBattery(&batt);

    TEST_ASSERT_EQUAL(BOARD_HEALTH_OUT_OF_RANGE, st);
}

void test_Battery_NullPointer(void)
{
	setUp_board();
    BoardHealthStatus_t st = BoardHealth_ReadBattery(NULL);
    TEST_ASSERT_EQUAL(BOARD_HEALTH_ADC_ERROR, st);
}

void test_TaskStep_ValidUpdate(void)
{
	setUp_board();
    BoardHealth_TaskStep();

    TEST_ASSERT_EQUAL(1000U, last_snapshot_written.task_last_run_ms);
}

void test_TaskStep_TickUpdates(void)
{
	setUp_board();
    mock_tick = 2000U;
    BoardHealth_TaskStep();

    TEST_ASSERT_EQUAL(2000U, last_snapshot_written.task_last_run_ms);
}

void test_board_health(void){
	RUN_TEST(test_Temperature_OK);
	RUN_TEST(test_Temperature_NullPointer);
	RUN_TEST(test_Temperature_VrefZero);
	RUN_TEST(test_Battery_OK);
	RUN_TEST(test_Battery_OutOfRange);
	RUN_TEST(test_Battery_NullPointer);
	RUN_TEST(test_TaskStep_ValidUpdate);
	RUN_TEST(test_TaskStep_TickUpdates);
}

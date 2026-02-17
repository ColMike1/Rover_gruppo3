#include "unity.h"
#include "imu_task.h"
#include "imu_i2c.h"

extern int snapshot_write_called;
extern IMUI2CStatus_t imu_read_result;

void setUp_imu(void)
{
    snapshot_write_called = 0;
}

void tearDown_imu(void) {}

void test_IMU_TaskStep_success(void)
{
	setUp_imu();
    imu_read_result = IMU_I2C_COMPLETE;

    IMU_TaskStep();

    TEST_ASSERT_EQUAL(1, snapshot_write_called);
}

void test_IMU_TaskStep_fail(void)
{
	setUp_imu();
    imu_read_result = IMU_I2C_ERROR;

    IMU_TaskStep();

    TEST_ASSERT_EQUAL(1, snapshot_write_called);
}

void test_imu(void){
	RUN_TEST(test_IMU_TaskStep_success);
	RUN_TEST(test_IMU_TaskStep_fail);
}

#include "unity.h"
#include "supervisor_task.h"
#include "supervisor_snapshot.h"
#include "gpio.h"
#include "sonar_snapshot.h"
#include "rx_snapshot.h"
#include "actuation_step.h"

extern uint32_t fake_tick;
extern GPIO_PinState last_gpio_state;

void test_NormalOperation_Flow(void) {
    RxSnapshot_t my_rx = {0};
    my_rx.payload.alive_counter = 1;
    RxSnapshot_Write(&my_rx);

    SonarSnapshot_t my_son = {0};
    my_son.dist_cm[0] = 150.0f;
    SonarSnapshot_Write(&my_son);

    fake_tick += 100;

    Supervisor_TaskStep();

    SupervisorSnapshot_t out_sup;
    SupervisorSnapshot_Read(&out_sup);

    TEST_ASSERT_EQUAL_UINT32(fake_tick, out_sup.task_last_run_ms);

    TEST_ASSERT_EQUAL(0, out_sup.critical_mask);
}

void test_EmergencyStop_Activation(void) {
    SonarSnapshot_t critical_son = {0};
    critical_son.dist_cm[0] = 5.0f;
    SonarSnapshot_Write(&critical_son);

    Supervisor_TaskStep();

    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, last_gpio_state);
}

void test_supervisor(void){
	RUN_TEST(test_NormalOperation_Flow);
	RUN_TEST(test_EmergencyStop_Activation);
}

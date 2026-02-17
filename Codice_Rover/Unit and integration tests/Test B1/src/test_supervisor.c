#include "unity.h"
#include "supervisor_task.h"
#include "supervisorB1.h"
#include "supervisor_snapshot.h"
#include "gpio.h"
#include <string.h>

extern ExtU_SupervisorB1_T SupervisorB1_U;
extern ExtY_SupervisorB1_T SupervisorB1_Y;

extern GPIO_PinState estop_state;
extern GPIO_PinState relay_state;
extern BoardHealthSnapshot_t bh_mock;
extern EncoderSnapshot_t mock_enc_data;
extern RxSnapshot_t mock_rx;
extern SupervisorSnapshot_t mock_sup_data;
extern uint32_t mock_tick;

void setUp_supervisor(void) {
    mock_tick = 0;
    memset(&bh_mock, 0, sizeof(BoardHealthSnapshot_t));
    memset(&mock_enc_data, 0, sizeof(EncoderSnapshot_t));
    memset(&mock_rx, 0, sizeof(RxSnapshot_t));
    memset(&mock_sup_data, 0, sizeof(SupervisorSnapshot_t));
    bh_mock.battery_pct = 80.0f;
    bh_mock.temperature_degC = 25.0f;
    Supervisor_TaskInit();
}

/**
 * @brief Test: Verifica che il sistema parta in stato di sicurezza (E-Stop attivo).
 */
void test_Supervisor_InitialSafeState(void) {
    setUp_supervisor();

    Supervisor_TaskStep();


    TEST_ASSERT_EQUAL(GPIO_PIN_SET, estop_state);
    TEST_ASSERT_TRUE(mock_sup_data.isBoardActuating);
}

/**
 * @brief Test: Reazione a un errore critico della batteria.
 */
void test_Supervisor_BatteryCriticalShutdown(void) {
    setUp_supervisor();

    bh_mock.battery_pct = 0.0f;
    bh_mock.temperature_degC = 25.0f;

    for(int i = 0; i < 600; i++) {
        mock_tick += 20;

        bh_mock.batt_last_valid_ms = mock_tick;
        mock_rx.data_last_valid_ms = mock_tick;
        mock_rx.payload.alive_counter++;
        Supervisor_TaskStep();
    }

    TEST_ASSERT_EQUAL_INT(GPIO_PIN_RESET, estop_state);
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, relay_state);
}

/**
 * @brief Test: Watchdog Board 2 (Liveliness).
 * Se la Board 2 smette di rispondere, il sistema deve andare in sicurezza.
 */
void test_Supervisor_Board2_Timeout(void) {
    setUp_supervisor();

    mock_rx.payload.alive_counter = 1;
    mock_tick = 1000;
    Supervisor_TaskStep();

    mock_tick = 11000;
    Supervisor_TaskStep();

    TEST_ASSERT_EQUAL_UINT32(1000, SupervisorB1_U.last_valid_b2_ms);
    TEST_ASSERT_EQUAL_UINT32(11000, SupervisorB1_U.now_ms);

    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, estop_state);
}

/**
 * @brief Test: Propagazione riferimenti in condizioni normali.
 */
void test_Supervisor_NormalOperation(void) {
    setUp_supervisor();

    mock_rx.payload.alive_counter = 5;
    mock_rx.payload.btn1 = 1;

    for(int i =0; i < 250; i++){
    	mock_tick += 20;
    	bh_mock.batt_last_valid_ms = mock_tick;
    	bh_mock.temp_last_valid_ms = mock_tick;
    	bh_mock.task_last_run_ms = mock_tick;
    	mock_rx.data_last_valid_ms = mock_tick;
    	mock_rx.payload.alive_counter++;
    	SupervisorB1_U.now_ms = mock_tick;
    	Supervisor_TaskStep();
    }

    TEST_ASSERT_EQUAL(GPIO_PIN_SET, relay_state);
}

void test_supervisor(void) {
    RUN_TEST(test_Supervisor_InitialSafeState);
    RUN_TEST(test_Supervisor_BatteryCriticalShutdown);
    RUN_TEST(test_Supervisor_Board2_Timeout);
    RUN_TEST(test_Supervisor_NormalOperation);
}

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "comm_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32_hal.h"
#include "rx_snapshot.h"
#include "supervisor_snapshot.h"
#include "ble_controller_snapshot.h"
#include "imu_snapshot.h"



extern uint8_t rx_buf[256];
extern uint16_t rx_wr;


RxSnapshot_t captured_rx_snap;
SupervisorSnapshot_t last_written_sup;


void Mock_UART_InjectByte(uint8_t b) {
    rx_buf[rx_wr] = b;
    rx_wr = (rx_wr + 1) % 256;
}

void Mock_UART_Clear(void) {
    extern uint16_t rx_rd, rx_wr;
    rx_rd = rx_wr = 0;
}

void RxSnapshot_Write(RxSnapshot_t *snap) {
    if (snap) captured_rx_snap = *snap;
}

void RxSnapshot_Read(RxSnapshot_t* dest) {
    if (dest != NULL) {
        memcpy(dest, &captured_rx_snap, sizeof(RxSnapshot_t));
    }
}

void SupervisorSnapshot_Read(SupervisorSnapshot_t *snap) {
    if (snap) {
    	memcpy(snap, &last_written_sup, sizeof(SupervisorSnapshot_t));
    }
}

void BleControllerSnapshot_Read(BleControllerSnapshot_t *snap) {
    if (snap) memset(snap, 0, sizeof(BleControllerSnapshot_t));
}

void IMUSnapshot_Read(IMUSnapshot_t *snap) {
    if (snap) memset(snap, 0, sizeof(IMUSnapshot_t));
}



void vTaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken) {}

uint16_t crc16_ccitt(const uint8_t *buf, uint16_t len) {
    return 0x1234;
}

void SupervisorSnapshot_Write(SupervisorSnapshot_t* src) {
    if (src != NULL) {
        memcpy(&last_written_sup, src, sizeof(SupervisorSnapshot_t));
    }
}

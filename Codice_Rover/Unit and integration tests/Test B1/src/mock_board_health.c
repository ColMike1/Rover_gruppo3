#include <stdint.h>
#include <string.h>
#include "stm32_hal.h"
#include "board_health_snapshot.h"


ADC_HandleTypeDef hadc1;
BoardHealthSnapshot_t last_snapshot_written;

extern uint8_t batt_ma_idx, batt_ma_cnt;
extern float batt_ma_buf[10];

HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length) {
    return HAL_OK;
}

void BoardHealthSnapshot_Write(BoardHealthSnapshot_t* s) {
	last_snapshot_written = *s;
}

void Mock_Reset_Battery_Filter(void) {
    batt_ma_idx = 0;
    batt_ma_cnt = 0;
    memset(batt_ma_buf, 0, sizeof(batt_ma_buf));
}

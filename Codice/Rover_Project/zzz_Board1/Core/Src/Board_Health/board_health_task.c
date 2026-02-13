/*
 * board_health_task.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Sterm
 */



#include "board_health/board_health.h"

#include "snapshot/board_health_snapshot.h"
#include "cmsis_os2.h"



void BoardHealth_TaskInit(void)
{
    BoardHealthADC_Init();
}

void BoardHealth_TaskStep(void)
{

  static BoardHealthSnapshot_t snap;
  float temp_degC;
  float batt_pct;

  BoardHealthStatus_t temp_st;
  BoardHealthStatus_t batt_st;

  temp_st = BoardHealth_ReadTemperature(&temp_degC);
  batt_st = BoardHealth_ReadBattery(&batt_pct);


  uint32_t now = osKernelGetTickCount();
  snap.task_last_run_ms = now;


  if (temp_st == BOARD_HEALTH_OK)
  {
      snap.temperature_degC = temp_degC;
      snap.temp_last_valid_ms = now;
  }

  if (batt_st == BOARD_HEALTH_OK)
  {
      snap.battery_pct = batt_pct;
      snap.batt_last_valid_ms = now;
  }


  BoardHealthSnapshot_Write(&snap);
}

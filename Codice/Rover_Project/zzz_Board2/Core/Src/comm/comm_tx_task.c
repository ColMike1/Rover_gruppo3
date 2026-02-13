/*
 * comm_tx_task.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */



#include "comm/comm_tx_task.h"
#include "comm/comm_pack.h"
#include "comm/comm_uart.h"

#include "shared_resources/comm_message_structures.h"

#include "snapshot/ble_controller_snapshot.h"
#include "snapshot/supervisor_snapshot.h"
#include "snapshot/imu_snapshot.h"

#define TX_FRAME_LEN (sizeof(CommFrameB2_t))


void Tx_TaskStep(void)
{
    SupervisorSnapshot_t sup;
    BleControllerSnapshot_t ble;
    IMUSnapshot_t imu;

    static uint8_t tx_buf[TX_FRAME_LEN];

    SupervisorSnapshot_Read(&sup);
    BleControllerSnapshot_Read(&ble);
    IMUSnapshot_Read(&imu);

    uint16_t tx_len = CommPack_BuildB2Tx(tx_buf, TX_FRAME_LEN, &ble, &sup, &imu);

    if (tx_len == TX_FRAME_LEN)
    {
        CommUart_Send(tx_buf, tx_len);
    }

}


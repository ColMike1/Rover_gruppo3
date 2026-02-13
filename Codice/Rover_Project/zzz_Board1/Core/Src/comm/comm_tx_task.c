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

#include "snapshot/encoder_snapshot.h"
#include "snapshot/supervisor_snapshot.h"


#define TX_FRAME_LEN (sizeof(CommFrameB1_t))


void Tx_TaskStep(void)
{
  EncoderSnapshot_t enc;
  SupervisorSnapshot_t sup;

  static uint8_t tx_buf[TX_FRAME_LEN];


  /* === Read snapshots === */
  EncoderSnapshot_Read(&enc);
  SupervisorSnapshot_Read(&sup);

  /* === Build frame === */
  uint16_t tx_len = CommPack_BuildB1Tx(tx_buf, TX_FRAME_LEN, &enc, &sup);

  /* === Send === */
  if (tx_len == TX_FRAME_LEN)
  {
    CommUart_Send(tx_buf, tx_len);
  }
}

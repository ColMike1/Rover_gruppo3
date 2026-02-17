/*
 * comm_pack.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */

#include "comm_pack.h"
#include "comm_message_structures.h"
#include "comm_crc.h"
#include <string.h>


/* ================= PACK FUNCTION ================= */

uint16_t CommPack_BuildB1Tx(uint8_t *buf,
                                uint16_t max_len,
                                const EncoderSnapshot_t *enc,
                                const SupervisorSnapshot_t *sup)
{
  if (!buf || !enc || !sup)
    return 0;

  /* frame sizes */
  const uint16_t HEADER_LEN = sizeof(CommFrameHeader_t);
  const uint16_t PAYLOAD_LEN = sizeof(CommPayloadB1_t);
  const uint16_t CRC_LEN = sizeof(uint16_t);
  const uint16_t FRAME_LEN = sizeof(CommFrameB1_t);

  if (max_len < FRAME_LEN)
    return 0;

  static uint16_t tx_seq = 0;

  uint16_t i = 0;


  /* ---------- Header ---------- */
  CommFrameHeader_t hdr;

  hdr.payload_len  = PAYLOAD_LEN;
  hdr.seq          = ++tx_seq;
  hdr.timestamp_ms = sup->task_last_run_ms;
  hdr.msg_id = 0xAA55;

  memcpy(&buf[i], &hdr, HEADER_LEN);
  i += HEADER_LEN;


  /* ---------- Payload ---------- */
  CommPayloadB1_t pl;
  for (int i = 0; i < 4; i++)
  {
      pl.wheel_speed_rpm[i] = enc->wheel_speed_rpm[i];
  }
  pl.degraded_mask  = sup->degraded_mask;
  pl.critical_mask  = sup->critical_mask;
  pl.alive_counter = sup->alive_counter;

  memcpy(&buf[i], &pl, PAYLOAD_LEN);
  i += PAYLOAD_LEN;


  /* ---------- CRC ---------- */
  uint16_t crc = crc16_ccitt(buf, i);
  memcpy(&buf[i], &crc, CRC_LEN);
  i += CRC_LEN;

  return i;   /* lunghezza frame valido */
}


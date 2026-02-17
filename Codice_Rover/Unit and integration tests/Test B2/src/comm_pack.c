/**
 * @file comm_pack.c
 * @brief Serializzazione del frame TX board2 -> board1.
 */
#include <string.h>
#include "comm_pack.h"
#include "comm_crc.h"
#include "comm_message_structures.h"

/** @brief Identificativo messaggio Board2 -> Board1. */
#define COMM_B2_MSG_ID 0xAA56U

/**
 * @brief Costruisce un frame completo Board2 -> Board1.
 *
 * Il frame e' composto da header, payload e CRC16-CCITT.
 *
 * @param[out] buf Buffer di output.
 * @param[in] max_len Dimensione massima del buffer.
 * @param[in] ble Snapshot BLE.
 * @param[in] sup Snapshot supervisore.
 * @param[in] imu Snapshot IMU.
 * @return Lunghezza del frame costruito, oppure 0 in caso di errore.
 */
uint16_t CommPack_BuildB2Tx(uint8_t *buf, uint16_t max_len, const BleControllerSnapshot_t *ble, const SupervisorSnapshot_t *sup, const IMUSnapshot_t *imu)
{
    if (!buf || !sup || !ble || !imu)
    {
        return 0U;
    }

    /* Dimensioni dei blocchi che compongono il frame. */
    const uint16_t HEADER_LEN = sizeof(CommFrameHeader_t);
    const uint16_t PAYLOAD_LEN = sizeof(CommPayloadB2_t);
    const uint16_t CRC_LEN = sizeof(uint16_t);
    const uint16_t FRAME_LEN = sizeof(CommFrameB2_t);

    if (max_len < FRAME_LEN)
    {
        return 0U;
    }

    static uint16_t tx_seq = 0U;

    uint16_t i = 0U;

    /* Header frame */
    CommFrameHeader_t hdr;

    hdr.payload_len  = PAYLOAD_LEN;
    hdr.seq          = ++tx_seq;
    hdr.timestamp_ms = sup->task_last_run_ms;
    hdr.msg_id = COMM_B2_MSG_ID;

    memcpy(&buf[i], &hdr, HEADER_LEN);
    i += HEADER_LEN;

    /* Payload applicativo */
    CommPayloadB2_t pl;

    pl.command = sup->command;
    pl.degraded_mask = sup->degraded_mask;
    pl.critical_mask = sup->critical_mask;
    pl.isMotionConsistent = sup->isMotionConsistent;
    pl.yaw = imu->yaw;
    pl.x_norm = ble->bx_norm;
    pl.y_norm = ble->ay_norm;
    pl.btn1 = ble->btn1;
    pl.btn2 = ble->btn2;

    /* Contatore alive trasmesso a board 1. */
    pl.alive_counter  = sup->alive_counter;

    memcpy(&buf[i], &pl, PAYLOAD_LEN);
    i += PAYLOAD_LEN;

    /* CRC finale */
    uint16_t crc = crc16_ccitt(buf, i);
    memcpy(&buf[i], &crc, CRC_LEN);
    i += CRC_LEN;

    return i;  /* == FRAME_LEN */
}


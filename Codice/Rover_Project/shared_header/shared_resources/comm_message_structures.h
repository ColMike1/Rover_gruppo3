/*
 * comm_mesage_structures.h
 *
 *  Created on: Jan 17, 2026
 *      Author: Sterm
 */

#ifndef COMM_MESSAGE_STRUCTURES_H_
#define COMM_MESSAGE_STRUCTURES_H_

#include <stdint.h>
#include "shared_resources/imu_coherence_status.h"
#include "shared_resources/supervisor_command.h"
/* ===== Common frame header ===== */
typedef struct __attribute__((packed))
{
    uint16_t msg_id;
    uint16_t payload_len;
    uint16_t seq;
    uint32_t timestamp_ms;
} CommFrameHeader_t;



/* ===== Payload Board1 -> Board2 ===== */
typedef struct __attribute__((packed))
{
    float wheel_speed_rpm[4];
    uint32_t degraded_mask;
    uint32_t critical_mask;
    uint32_t alive_counter;
} CommPayloadB1_t;

/* ===== Payload Board2 -> Board1 ===== */
typedef struct __attribute__((packed))
{
	SupervisorCommand_t command;
    uint32_t degraded_mask;
    uint32_t critical_mask;
    ImuCoherenceStatus_t imu_coherence_status;

    float yaw;
    float x_norm;
    float y_norm;

    uint32_t alive_counter;
} CommPayloadB2_t;

/* ===== Full frames ===== */
typedef struct __attribute__((packed))
{
    CommFrameHeader_t header;
    CommPayloadB1_t payload;
    uint16_t crc;
} CommFrameB1_t;

typedef struct __attribute__((packed))
{
    CommFrameHeader_t header;
    CommPayloadB2_t payload;
    uint16_t crc;
} CommFrameB2_t;


#endif /* COMM_MESSAGE_STRUCTURES_H_ */

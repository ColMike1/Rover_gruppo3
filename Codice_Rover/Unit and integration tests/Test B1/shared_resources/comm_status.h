/*
 * comm_status.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef COMM_STATUS_H_
#define COMM_STATUS_H_

typedef enum
{
    COMM_UNPACK_OK = 0,
    COMM_UNPACK_NULL,
    COMM_UNPACK_LEN,
    COMM_UNPACK_CRC
} CommUnpackStatus_t;

#endif /* COMM_STATUS_H_ */

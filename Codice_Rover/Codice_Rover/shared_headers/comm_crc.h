/*
 * comm_crc.h
 *
 *  Created on: Jan 17, 2026
 *      Author: Sterm
 */

#ifndef COMM_CRC_H_
#define COMM_CRC_H_

#include <stdint.h>

static inline uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    const uint16_t poly = 0x1021;

    for (uint16_t i = 0; i < len; i++)
    {
        crc ^= ((uint16_t)data[i] << 8);
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc & 0x8000) ? (crc << 1) ^ poly : (crc << 1);
        }
    }
    return crc;
}

#endif /* COMM_CRC_H_ */

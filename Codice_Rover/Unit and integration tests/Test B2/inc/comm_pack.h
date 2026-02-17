/*
 * comm_pack.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */

#ifndef INC_COMM_COMM_PACK_H_
#define INC_COMM_COMM_PACK_H_


#include <stdint.h>

#include "supervisor_snapshot.h"
#include "ble_controller_snapshot.h"
#include "imu_snapshot.h"

/**
 * @brief Build Board2 -> Board1 supervisor frame
 *
 * @param[out] buf Output buffer
 * @param[in]  max_len Buffer size
 * @param[in]  ble BLE controller snapshot
 * @param[in]  sup Supervisor snapshot
 * @param[in]  imu IMU snapshot
 *
 * @return Frame length in bytes (0 on error)
 */
uint16_t CommPack_BuildB2Tx(uint8_t *buf,
                                    uint16_t max_len,
                                    const BleControllerSnapshot_t *ble,
                                    const SupervisorSnapshot_t *sup,
									const IMUSnapshot_t *imu);



#endif /* INC_COMM_COMM_PACK_H_ */

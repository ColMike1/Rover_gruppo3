/**
 * @file comm_pack.h
 * @brief API di serializzazione frame in uscita da board 2.
 */

#ifndef INC_COMM_COMM_PACK_H_
#define INC_COMM_COMM_PACK_H_


#include <stdint.h>

#include "snapshot/supervisor_snapshot.h"
#include "snapshot/ble_controller_snapshot.h"
#include "snapshot/imu_snapshot.h"

/**
 * @brief Costruisce un frame Board2 -> Board1.
 * @param[out] buf Buffer di uscita.
 * @param[in]  max_len Dimensione del buffer di uscita.
 * @param[in]  ble Snapshot BLE.
 * @param[in]  sup Snapshot supervisore.
 * @param[in]  imu Snapshot IMU.
 * @return Lunghezza frame in byte (0 in caso di errore).
 */
uint16_t CommPack_BuildB2Tx(uint8_t *buf, uint16_t max_len, const BleControllerSnapshot_t *ble, const SupervisorSnapshot_t *sup, const IMUSnapshot_t *imu);



#endif /* INC_COMM_COMM_PACK_H_ */

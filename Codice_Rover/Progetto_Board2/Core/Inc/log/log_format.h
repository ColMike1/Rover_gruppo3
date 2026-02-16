/**
 * @file log_format.h
 * @brief API di formattazione degli snapshot diagnostici.
 */

#ifndef INC_LOG_LOG_FORMAT_H_
#define INC_LOG_LOG_FORMAT_H_

#include "snapshot/ble_controller_snapshot.h"
#include "snapshot/imu_snapshot.h"
#include "snapshot/sonar_snapshot.h"
#include "snapshot/rx_snapshot.h"

/**
 * @brief Costruisce una stringa di log completa a partire dagli snapshot.
 * @param buf Buffer di output.
 * @param buf_len Lunghezza del buffer di output.
 * @param ble Snapshot BLE.
 * @param imu Snapshot IMU.
 * @param sonar Snapshot sonar.
 * @param rx Snapshot RX.
 */
void Log_FormatSnapshot(char *buf, unsigned buf_len, const BleControllerSnapshot_t *ble, const IMUSnapshot_t *imu, const SonarSnapshot_t *sonar, const RxSnapshot_t *rx);



#endif /* INC_LOG_LOG_FORMAT_H_ */

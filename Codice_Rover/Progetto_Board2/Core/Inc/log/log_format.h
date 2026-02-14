/*
 * log_format.h
 *
 *  Created on: Jan 19, 2026
 *      Author: Sterm
 */

#ifndef INC_LOG_LOG_FORMAT_H_
#define INC_LOG_LOG_FORMAT_H_

#include "snapshot/ble_controller_snapshot.h"
#include "snapshot/imu_snapshot.h"
#include "snapshot/sonar_snapshot.h"
#include "snapshot/rx_snapshot.h"

void Log_FormatSnapshot(char *buf,
                        unsigned buf_len,
                        const BleControllerSnapshot_t *ble,
                        const IMUSnapshot_t *imu,
                        const SonarSnapshot_t *sonar,
						const RxSnapshot_t *rx);


#endif /* INC_LOG_LOG_FORMAT_H_ */

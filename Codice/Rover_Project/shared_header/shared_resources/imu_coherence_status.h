/*
 * imu_coherence_status.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef IMU_COHERENCE_STATUS_H_
#define IMU_COHERENCE_STATUS_H_

typedef enum
{
  IMU_COHERENT = 0,
  IMU_INCOHERENT_LEFT,
  IMU_INCOHERENT_RIGHT
} ImuCoherenceStatus_t;

#endif /* IMU_COHERENCE_STATUS_H_ */

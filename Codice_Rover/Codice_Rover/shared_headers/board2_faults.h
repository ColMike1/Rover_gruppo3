/*
 * board2_faults.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef BOARD2_FAULTS_H_
#define BOARD2_FAULTS_H_


#define FAULT_NONE         0U
#define FAULT_BLE        (1U << 0)  /* Temperature */
#define FAULT_IMU        (1U << 1)  /* Battery */
#define FAULT_RX         (1U << 2)  /* Rx message from board 1 */
#define FAULT_B1_SUP          (1U << 3)  /* RX communication */




#endif /* BOARD2_FAULTS_H_ */

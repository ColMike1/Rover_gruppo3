/*
 * board1_faults.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef BOARD1_FAULTS_H_
#define BOARD1_FAULTS_H_


#define FAULT_NONE         0U
#define FAULT_TEMP        (1U << 0)  /* Temperature */
#define FAULT_BATT        (1U << 1)  /* Battery */
#define FAULT_RX          (1U << 2)  /* RX communication */

#define FAULT_WHEEL_FL    (1U << 3)  /* Front Left wheel */
#define FAULT_WHEEL_FR    (1U << 4)  /* Front Right wheel */
#define FAULT_WHEEL_RL    (1U << 5)  /* Rear Left wheel */
#define FAULT_WHEEL_RR    (1U << 6)  /* Rear Right wheel */

#define FAULT_B2_SUP      (1U << 7)  /* Board2 supervisor */


#endif /* BOARD1_FAULTS_H_ */

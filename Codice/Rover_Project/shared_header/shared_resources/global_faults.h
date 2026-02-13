/*
 * global_faults.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef GLOBAL_FAULTS_H_
#define GLOBAL_FAULTS_H_


#define FAULT_NONE                0U

#define FAULT_BOARD1_FAILED       (1U << 0)
#define FAULT_BOARD2_FAILED       (1U << 1)

#define FAULT_MOTOR_DRIVER        (1U << 2)
#define FAULT_ENCODER_UNRELIABLE  (1U << 3)
#define FAULT_IMU_FAILED          (1U << 4)
#define FAULT_CMD_UNAVAILABLE     (1U << 5)

#define FAULT_OVERTEMP            (1U << 6)
#define FAULT_BATT_LOW            (1U << 7)


#endif /* GLOBAL_FAULTS_H_ */

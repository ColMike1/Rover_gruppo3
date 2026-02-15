/**
 * @file imu_task.h
 * @brief API del task periodico IMU.
 */

#ifndef INC_IMU_IMU_TASK_H_
#define INC_IMU_IMU_TASK_H_

/**
 * @brief Inizializza il task IMU.
 */
void IMU_TaskInit(void);

/**
 * @brief Esegue uno step del task IMU.
 */
void IMU_TaskStep(void);

#endif /* INC_IMU_IMU_TASK_H_ */

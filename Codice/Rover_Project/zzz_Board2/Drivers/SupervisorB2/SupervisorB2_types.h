/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB2_types.h
 *
 * Code generated for Simulink model 'SupervisorB2'.
 *
 * Model version                  : 1.174
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Thu Feb 12 13:14:47 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef SupervisorB2_types_h_
#define SupervisorB2_types_h_
#include "snapshot/imu_snapshot.h"
#include "shared_resources/comm_message_structures.h"
#include "shared_resources/comm_status.h"
#include "snapshot/rx_snapshot.h"
#include "snapshot/sonar_snapshot.h"
#include "snapshot/ble_controller_snapshot.h"
#include "rtwtypes.h"
#include "shared_resources/supervisor_command.h"
#include "shared_resources\imu_coherence_status.h"
#ifndef DEFINED_TYPEDEF_FOR_RxStatus_
#define DEFINED_TYPEDEF_FOR_RxStatus_

typedef enum {
  RX_OK = 0,                           /* Default value */
  RX_DEGRADED,
  RX_CRITICAL
} RxStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SupervisorStatus_
#define DEFINED_TYPEDEF_FOR_SupervisorStatus_

typedef enum {
  SUPERVISOR_OK = 0,                   /* Default value */
  SUPERVISOR_DEGRADED,
  SUPERVISOR_CRITICAL
} SupervisorStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_EspStatus_
#define DEFINED_TYPEDEF_FOR_EspStatus_

typedef enum {
  ESP_I2C_OK = 0,                      /* Default value */
  ESP_I2C_DEGRADED,
  ESP_I2C_CRITICAL
} EspStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SafetyStatus_
#define DEFINED_TYPEDEF_FOR_SafetyStatus_

typedef enum {
  SAFETY_OK = 0,                       /* Default value */
  SAFETY_DEGRADED,
  SAFETY_CRITICAL
} SafetyStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ImuStatus_
#define DEFINED_TYPEDEF_FOR_ImuStatus_

typedef enum {
  IMU_I2C_OK = 0,                      /* Default value */
  IMU_I2C_DEGRADED,
  IMU_I2C_CRITICAL
} ImuStatus;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_SupervisorB2_T RT_MODEL_SupervisorB2_T;

#endif                                 /* SupervisorB2_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

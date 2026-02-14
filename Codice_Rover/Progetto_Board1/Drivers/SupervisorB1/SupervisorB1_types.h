/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB1_types.h
 *
 * Code generated for Simulink model 'SupervisorB1'.
 *
 * Model version                  : 1.172
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sat Feb 14 15:37:03 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef SupervisorB1_types_h_
#define SupervisorB1_types_h_
#include "shared_headers/supervisor_command.h"
#include "shared_headers/comm_message_structures.h"
#include "shared_headers/comm_status.h"
#include "snapshot/rx_snapshot.h"
#include "snapshot/board_health_snapshot.h"
#include "snapshot/encoder_snapshot.h"
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_SafetyStatus_
#define DEFINED_TYPEDEF_FOR_SafetyStatus_

typedef enum {
  SAFETY_OK = 0,                       /* Default value */
  SAFETY_DEGRADED,
  SAFETY_CRITICAL
} SafetyStatus;

#endif

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

#ifndef DEFINED_TYPEDEF_FOR_TemperatureStatus_
#define DEFINED_TYPEDEF_FOR_TemperatureStatus_

typedef enum {
  TEMP_HEALTH_OK = 0,                  /* Default value */
  TEMP_HEALTH_DEGRADED,
  TEMP_HEALTH_CRITICAL
} TemperatureStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_BatteryStatus_
#define DEFINED_TYPEDEF_FOR_BatteryStatus_

typedef enum {
  BATTERY_HEALTH_OK = 0,               /* Default value */
  BATTERY_HEALTH_DEGRADED,
  BATTERY_HEALTH_CRITICAL
} BatteryStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_WheelStatus_
#define DEFINED_TYPEDEF_FOR_WheelStatus_

typedef enum {
  WHEEL_OK = 0,                        /* Default value */
  WHEEL_DEGRADED_ENCODER,
  WHEEL_CRITICAL_MOTOR
} WheelStatus;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_SupervisorB1_T RT_MODEL_SupervisorB1_T;

#endif                                 /* SupervisorB1_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

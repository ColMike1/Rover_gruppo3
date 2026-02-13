/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB1.h
 *
 * Code generated for Simulink model 'SupervisorB1'.
 *
 * Model version                  : 1.163
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Wed Feb 11 16:01:55 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef SupervisorB1_h_
#define SupervisorB1_h_
#ifndef SupervisorB1_COMMON_INCLUDES_
#define SupervisorB1_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* SupervisorB1_COMMON_INCLUDES_ */

#include "SupervisorB1_types.h"
#include "snapshot/rx_snapshot.h"
#include "snapshot/board_health_snapshot.h"
#include "snapshot/encoder_snapshot.h"
#include "shared_resources/supervisor_command.h"
#include "rtGetNaN.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  WheelStatus wheel_status[4];         /* '<Root>/Monitor Board Status' */
} B_SupervisorB1_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T buf[10];                      /* '<Root>/Monitor Board Status' */
  real_T idx;                          /* '<Root>/Monitor Board Status' */
  real_T count;                        /* '<Root>/Monitor Board Status' */
  WheelStatus UnitDelay_DSTATE[4];     /* '<Root>/Unit Delay' */
  real32_T v_user;                     /* '<Root>/Check rover safety state1' */
  uint32_T last_valid_prev;            /* '<Root>/Monitor Board Status' */
  uint32_T sum_dt;                     /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1;          /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_k;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_a;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_kv;       /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_2;          /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_e;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_m;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_l;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_i;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_2_f;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_h;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_o;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_p;        /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_ie;       /* '<Root>/Monitor Board Status' */
  uint32_T durationCounter_1_n;        /* '<Root>/Check rover safety state1' */
  uint32_T durationCounter_1_d;        /* '<Root>/B1_Failure_Fallback' */
  uint8_T is_active_c2_SupervisorB1;   /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorTemperature;       /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorBattery;           /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorFrontLeft;         /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorFrontRight;        /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorRearLeft;          /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorRearRight;         /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorBoard2Supervisor;  /* '<Root>/Monitor Board Status' */
  uint8_T is_MonitorRx;                /* '<Root>/Monitor Board Status' */
  uint8_T is_active_c3_SupervisorB1;   /* '<Root>/Check rover safety state1' */
  uint8_T is_c3_SupervisorB1;          /* '<Root>/Check rover safety state1' */
  uint8_T is_Procedura_frenata_di_emergen;/* '<Root>/Check rover safety state1' */
  uint8_T is_Stato_Marcia_rover;       /* '<Root>/Check rover safety state1' */
  uint8_T is_active_c7_SupervisorB1;   /* '<Root>/Check rover safety state' */
  uint8_T is_c7_SupervisorB1;          /* '<Root>/Check rover safety state' */
  uint8_T is_active_c4_SupervisorB1;   /* '<Root>/B1_Failure_Fallback' */
  uint8_T is_c4_SupervisorB1;          /* '<Root>/B1_Failure_Fallback' */
  boolean_T degraded_rx;               /* '<Root>/Monitor Board Status' */
  boolean_T degraded_sup;              /* '<Root>/Monitor Board Status' */
  boolean_T last_valid_prev_not_empty; /* '<Root>/Monitor Board Status' */
  boolean_T done180;                   /* '<Root>/Check rover safety state1' */
  boolean_T roverIsStopped;            /* '<Root>/Check rover safety state1' */
  boolean_T is_rotating180;            /* '<Root>/Check rover safety state1' */
  boolean_T invert_y;                  /* '<Root>/Check rover safety state1' */
  boolean_T start180;                  /* '<Root>/Check rover safety state1' */
  boolean_T estop_recover_enabled;     /* '<Root>/Check rover safety state1' */
} DW_SupervisorB1_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  RxSnapshot_t Board2_Data;            /* '<Root>/Board2_Data' */
  BoardHealthSnapshot_t Board_Health;  /* '<Root>/Board_Health' */
  EncoderSnapshot_t Encoder;           /* '<Root>/Encoder' */
  uint32_T now_ms;                     /* '<Root>/now_ms' */
  uint32_T last_valid_b2_ms;           /* '<Root>/last_valid_b2_ms' */
} ExtU_SupervisorB1_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T v_ref;                      /* '<Root>/v_ref' */
  real32_T omega_ref;                  /* '<Root>/omega_ref' */
  uint32_T critical_mask;              /* '<Root>/critical_mask' */
  uint32_T degraded_mask;              /* '<Root>/degraded_mask' */
  boolean_T give_b2_actuation;         /* '<Root>/give_b2_actuation' */
  boolean_T actuate_emergency_stop;    /* '<Root>/actuate_emergency_stop' */
  SupervisorCommand_t current_action;  /* '<Root>/current_action' */
  real32_T target_angle;               /* '<Root>/target_angle' */
  real32_T current_yaw_seen;           /* '<Root>/current_yaw_seen' */
} ExtY_SupervisorB1_T;

/* Real-time Model Data Structure */
struct tag_RTM_SupervisorB1_T {
  const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_SupervisorB1_T SupervisorB1_B;

/* Block states (default storage) */
extern DW_SupervisorB1_T SupervisorB1_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_SupervisorB1_T SupervisorB1_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SupervisorB1_T SupervisorB1_Y;

/* Model entry point functions */
extern void SupervisorB1_initialize(void);
extern void SupervisorB1_step(void);
extern void SupervisorB1_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SupervisorB1_T *const SupervisorB1_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SupervisorB1'
 * '<S1>'   : 'SupervisorB1/B1_Failure_Fallback'
 * '<S2>'   : 'SupervisorB1/Board 1 fault masks'
 * '<S3>'   : 'SupervisorB1/Check rover safety state'
 * '<S4>'   : 'SupervisorB1/Check rover safety state1'
 * '<S5>'   : 'SupervisorB1/MATLAB Function1'
 * '<S6>'   : 'SupervisorB1/Monitor Board Status'
 * '<S7>'   : 'SupervisorB1/Process B2 masks'
 */
#endif                                 /* SupervisorB1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

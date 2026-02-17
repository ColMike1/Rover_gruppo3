/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB2.h
 *
 * Code generated for Simulink model 'SupervisorB2'.
 *
 * Model version                  : 1.189
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sat Feb 14 15:33:13 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef SupervisorB2_h_
#define SupervisorB2_h_
#ifndef SupervisorB2_COMMON_INCLUDES_
#define SupervisorB2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* SupervisorB2_COMMON_INCLUDES_ */

#include "SupervisorB2_types.h"
#include "imu_snapshot.h"
#include "rx_snapshot.h"
#include "sonar_snapshot.h"
#include "ble_controller_snapshot.h"
#include "supervisor_command.h"
#include "rt_zcfcn.h"
#include "rtGetNaN.h"
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  SupervisorCommand_t B2Decision_c;    /* '<S14>/SuperB2_DEGB1_0' */
  SupervisorCommand_t B2Decision_h;    /* '<S1>/Generate B2 Decision' */
} B_SupervisorB2_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T buf[10];                      /* '<Root>/Monitor Board Local Status' */
  real_T idx;                          /* '<Root>/Monitor Board Local Status' */
  real_T count;                        /* '<Root>/Monitor Board Local Status' */
  real_T acc_enc;                      /* '<Root>/Imu-odometry coherence' */
  real_T acc_imu;                      /* '<Root>/Imu-odometry coherence' */
  real_T acc_enc_long;                 /* '<Root>/Imu-odometry coherence' */
  real_T acc_imu_long;                 /* '<Root>/Imu-odometry coherence' */
  real32_T targetAngle;                /* '<S14>/SuperB2_DEGB1_0' */
  real32_T angle_err;                  /* '<S1>/Rover motion state' */
  real32_T targetAngle_i;              /* '<S1>/Rover motion state' */
  real32_T v_user;                     /* '<S1>/Rover motion state' */
  int32_T hist_hit[10];                /* '<Root>/Imu-odometry coherence' */
  int32_T hist_idx;                    /* '<Root>/Imu-odometry coherence' */
  uint32_T last_valid_prev;            /* '<Root>/Monitor Board Local Status' */
  uint32_T sum_dt;                     /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1;          /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_d;        /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_a;        /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_f;        /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_g;        /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_e;        /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_gb;       /* '<Root>/Monitor Board Local Status' */
  uint32_T durationCounter_1_i;        /* '<Root>/Monitor Board Local Status' */
  uint32_T last_enc_ms;                /* '<Root>/Imu-odometry coherence' */
  uint32_T last_imu_ms;                /* '<Root>/Imu-odometry coherence' */
  uint32_T win_start_ms;               /* '<Root>/Imu-odometry coherence' */
  uint32_T long_start_ms;              /* '<Root>/Imu-odometry coherence' */
  uint32_T previousTicks;              /* '<S14>/SuperB2_DEGB1_0' */
  uint32_T durationCounter_1_l;        /* '<Root>/Check for B1 failure' */
  uint32_T durationCounter_1_b;        /* '<S1>/Rover motion state' */
  uint8_T is_active_c13_SupervisorB2;  /* '<Root>/Monitor Board Local Status' */
  uint8_T is_MonitorBoard1Supervisor;  /* '<Root>/Monitor Board Local Status' */
  uint8_T is_MonitorBLE;               /* '<Root>/Monitor Board Local Status' */
  uint8_T is_MonitorIMU;               /* '<Root>/Monitor Board Local Status' */
  uint8_T is_MonitorRx;                /* '<Root>/Monitor Board Local Status' */
  uint8_T is_active_c3_SupervisorB2;   /* '<Root>/Generate Decision for B1' */
  uint8_T isClose1;                    /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T isClose2;                    /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T isClose3;                    /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T obj1Detected;                /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T obj2Detected;                /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T obj3Detected;                /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T stop;                        /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_c1_SupervisorB2;   /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_B2Decisions;       /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_B2Decisions;              /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_Sonars;            /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_distance_70;       /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_distance_70;              /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_distance_gt70;     /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S1;        /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S1;               /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S2;        /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S2;               /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S3;        /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S3;               /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_timers;            /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar12;      /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar12;             /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar21;      /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar21;             /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar23;      /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar23;             /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar32;      /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar32;             /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i1;          /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i2;          /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i3;          /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i4;          /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i5;          /* '<S14>/SuperB2_DEGB1_0' */
  uint8_T is_active_c4_SupervisorB2;   /* '<Root>/Check for B1 failure' */
  uint8_T is_c4_SupervisorB2;          /* '<Root>/Check for B1 failure' */
  uint8_T is_active_c2_SupervisorB2;   /* '<S1>/Rover motion state' */
  uint8_T is_Backward_mode_toggle;     /* '<S1>/Rover motion state' */
  uint8_T is_Combo_toggle_backward;    /* '<S1>/Rover motion state' */
  uint8_T is_Motion_Supervision;       /* '<S1>/Rover motion state' */
  uint8_T is_Procedura_emergency_stop; /* '<S1>/Rover motion state' */
  uint8_T is_Stato_marcia_rover;       /* '<S1>/Rover motion state' */
  uint8_T temporalCounter_i1_d;        /* '<S1>/Rover motion state' */
  uint8_T temporalCounter_i2_l;        /* '<S1>/Rover motion state' */
  uint8_T previousZC;                  /* '<S1>/Rover motion state' */
  uint8_T previousZC_f;                /* '<S1>/Rover motion state' */
  uint8_T previousZC_a;                /* '<S1>/Rover motion state' */
  uint8_T previousZC_av;               /* '<S1>/Rover motion state' */
  uint8_T is_active_c5_SupervisorB2;   /* '<S1>/Generate B2 Decision' */
  uint8_T is_active_c9_SupervisorB2;   /* '<S12>/SuperB2_DEGB1_1' */
  uint8_T is_c9_SupervisorB2;          /* '<S12>/SuperB2_DEGB1_1' */
  uint8_T is_active_c6_SupervisorB2;   /* '<S1>/Check rover safety state' */
  uint8_T is_c6_SupervisorB2;          /* '<S1>/Check rover safety state' */
  boolean_T degraded_sup;              /* '<Root>/Monitor Board Local Status' */
  boolean_T degraded_rx;               /* '<Root>/Monitor Board Local Status' */
  boolean_T degraded_imu;              /* '<Root>/Monitor Board Local Status' */
  boolean_T degraded_esp;              /* '<Root>/Monitor Board Local Status' */
  boolean_T last_valid_prev_not_empty; /* '<Root>/Monitor Board Local Status' */
  boolean_T init_not_empty;            /* '<Root>/Imu-odometry coherence' */
  boolean_T roverIsStopped;            /* '<S1>/Rover motion state' */
  boolean_T is_rotating180;            /* '<S1>/Rover motion state' */
  boolean_T invert_y;                  /* '<S1>/Rover motion state' */
  boolean_T estop_recover_enabled;     /* '<S1>/Rover motion state' */
  boolean_T backward_enabled;          /* '<S1>/Rover motion state' */
} DW_SupervisorB2_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState SFunction_edgeDetectionSignal_Z;/* '<S1>/Rover motion state' */
  ZCSigState SFunction_edgeDetectionSignal_m;/* '<S1>/Rover motion state' */
  ZCSigState SFunction_edgeDetectionSigna_mi;/* '<S1>/Rover motion state' */
  ZCSigState SFunction_edgeDetectionSign_mir;/* '<S1>/Rover motion state' */
} PrevZCX_SupervisorB2_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  IMUSnapshot_t IMU;                   /* '<Root>/IMU' */
  uint32_T now_ms;                     /* '<Root>/now_ms' */
  uint32_T last_valid_b1_ms;           /* '<Root>/last_valid_b1_ms' */
  RxSnapshot_t Board1_Data;            /* '<Root>/Board1_Data' */
  SonarSnapshot_t Sonars;              /* '<Root>/Sonars' */
  BleControllerSnapshot_t BLE;         /* '<Root>/BLE' */
} ExtU_SupervisorB2_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  SupervisorCommand_t B2Decision;      /* '<Root>/B2Decision' */
  uint32_T critical_mask;              /* '<Root>/critical_mask' */
  uint32_T degraded_mask;              /* '<Root>/degraded_mask' */
  boolean_T isMotionConsistent;        /* '<Root>/isMotionConsistent' */
  boolean_T autorized_to_send_command; /* '<Root>/autorized_to_send_command' */
  real32_T v_ref_actuation;            /* '<Root>/v_ref_actuation' */
  real32_T omega_ref_actuation;        /* '<Root>/omega_ref_actuation' */
  boolean_T actuate_emergency_stop;    /* '<Root>/actuate_emergency_stop' */
  SupervisorCommand_t current_action;  /* '<Root>/current_action' */
} ExtY_SupervisorB2_T;

/* Real-time Model Data Structure */
struct tag_RTM_SupervisorB2_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
  } Timing;
};

/* Block signals (default storage) */
extern B_SupervisorB2_T SupervisorB2_B;

/* Block states (default storage) */
extern DW_SupervisorB2_T SupervisorB2_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_SupervisorB2_T SupervisorB2_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_SupervisorB2_T SupervisorB2_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SupervisorB2_T SupervisorB2_Y;

/* Model entry point functions */
extern void SupervisorB2_initialize(void);
extern void SupervisorB2_step(void);
extern void SupervisorB2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SupervisorB2_T *const SupervisorB2_M;

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
 * '<Root>' : 'SupervisorB2'
 * '<S1>'   : 'SupervisorB2/Actuation decision and motion state'
 * '<S2>'   : 'SupervisorB2/Board 2 fault masks'
 * '<S3>'   : 'SupervisorB2/Check for B1 failure'
 * '<S4>'   : 'SupervisorB2/Classify b1 crit mask'
 * '<S5>'   : 'SupervisorB2/Classify b2 crit mask'
 * '<S6>'   : 'SupervisorB2/Generate Decision for B1'
 * '<S7>'   : 'SupervisorB2/Imu-odometry coherence'
 * '<S8>'   : 'SupervisorB2/Monitor Board Local Status'
 * '<S9>'   : 'SupervisorB2/Actuation decision and motion state/Check rover safety state'
 * '<S10>'  : 'SupervisorB2/Actuation decision and motion state/Generate B2 Decision'
 * '<S11>'  : 'SupervisorB2/Actuation decision and motion state/Rover motion state'
 * '<S12>'  : 'SupervisorB2/Actuation decision and motion state/Generate B2 Decision/superB2_DEGB1_1'
 * '<S13>'  : 'SupervisorB2/Actuation decision and motion state/Generate B2 Decision/superB2_DEGB1_1/SuperB2_DEGB1_1'
 * '<S14>'  : 'SupervisorB2/Generate Decision for B1/superB2_DEGB1_0'
 * '<S15>'  : 'SupervisorB2/Generate Decision for B1/superB2_DEGB1_0/SuperB2_DEGB1_0'
 */
#endif                                 /* SupervisorB2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

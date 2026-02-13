/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB2.h
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

#ifndef SupervisorB2_h_
#define SupervisorB2_h_
#ifndef SupervisorB2_COMMON_INCLUDES_
#define SupervisorB2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* SupervisorB2_COMMON_INCLUDES_ */

#include "SupervisorB2_types.h"
#include "snapshot/imu_snapshot.h"
#include "snapshot/rx_snapshot.h"
#include "snapshot/sonar_snapshot.h"
#include "snapshot/ble_controller_snapshot.h"
#include "shared_resources/supervisor_command.h"
#include "shared_resources\imu_coherence_status.h"
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
  SupervisorCommand_t B2Decision;      /* '<S4>/Generate Fallback Decision' */
  SupervisorCommand_t B2Decision_c;    /* '<S9>/SuperB2_DEGB1_0' */
} B_SupervisorB2_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T buf[10];                      /* '<Root>/Monitor Board Status1' */
  real_T idx;                          /* '<Root>/Monitor Board Status1' */
  real_T count;                        /* '<Root>/Monitor Board Status1' */
  real_T acc_enc;                      /* '<Root>/Imu-odometry coherence' */
  real_T acc_imu;                      /* '<Root>/Imu-odometry coherence' */
  real32_T targetAngle;                /* '<S4>/Check rover safety state1' */
  real32_T v_user;                     /* '<S4>/Check rover safety state1' */
  real32_T targetAngle_g;              /* '<S9>/SuperB2_DEGB1_0' */
  int32_T hist_hit[10];                /* '<Root>/Imu-odometry coherence' */
  int32_T hist_idx;                    /* '<Root>/Imu-odometry coherence' */
  uint32_T last_valid_prev;            /* '<Root>/Monitor Board Status1' */
  uint32_T sum_dt;                     /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1;          /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_d;        /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_a;        /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_f;        /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_g;        /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_e;        /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_gb;       /* '<Root>/Monitor Board Status1' */
  uint32_T durationCounter_1_i;        /* '<Root>/Monitor Board Status1' */
  uint32_T last_enc_ms;                /* '<Root>/Imu-odometry coherence' */
  uint32_T last_imu_ms;                /* '<Root>/Imu-odometry coherence' */
  uint32_T win_start_ms;               /* '<Root>/Imu-odometry coherence' */
  uint32_T durationCounter_1_j;        /* '<S4>/Check rover safety state1' */
  uint32_T previousTicks;              /* '<S9>/SuperB2_DEGB1_0' */
  uint32_T durationCounter_1_l;        /* '<Root>/B1_Failure_Fallback' */
  uint8_T is_active_c13_SupervisorB2;  /* '<Root>/Monitor Board Status1' */
  uint8_T is_MonitorBoard1Supervisor;  /* '<Root>/Monitor Board Status1' */
  uint8_T is_MonitorBLE;               /* '<Root>/Monitor Board Status1' */
  uint8_T is_MonitorIMU;               /* '<Root>/Monitor Board Status1' */
  uint8_T is_MonitorRx;                /* '<Root>/Monitor Board Status1' */
  uint8_T is_active_c5_SupervisorB2;   /* '<S4>/Generate Fallback Decision' */
  uint8_T is_active_c9_SupervisorB2;   /* '<S14>/SuperB2_DEGB1_1' */
  uint8_T is_c9_SupervisorB2;          /* '<S14>/SuperB2_DEGB1_1' */
  uint8_T is_active_c2_SupervisorB2;   /* '<S4>/Check rover safety state1' */
  uint8_T is_c2_SupervisorB2;          /* '<S4>/Check rover safety state1' */
  uint8_T is_Procedura_frenata_di_emergen;/* '<S4>/Check rover safety state1' */
  uint8_T is_Stato_Marcia_rover;       /* '<S4>/Check rover safety state1' */
  uint8_T temporalCounter_i1;          /* '<S4>/Check rover safety state1' */
  uint8_T is_active_c6_SupervisorB2;   /* '<S4>/Check rover safety state' */
  uint8_T is_c6_SupervisorB2;          /* '<S4>/Check rover safety state' */
  uint8_T is_active_c3_SupervisorB2;   /* '<Root>/Generate Decision for B1' */
  uint8_T isClose1;                    /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T isClose2;                    /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T isClose3;                    /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T obj1Detected;                /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T obj2Detected;                /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T obj3Detected;                /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T stop;                        /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_c1_SupervisorB2;   /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_B2Decisions;       /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_B2Decisions;              /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_Sonars;            /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_distance_70;       /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_distance_70;              /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_distance_gt70;     /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S1;        /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S1;               /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S2;        /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S2;               /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_Waiting_S3;        /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_Waiting_S3;               /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_timers;            /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar12;      /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar12;             /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar21;      /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar21;             /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar23;      /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar23;             /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_timerSonar32;      /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_timerSonar32;             /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i1_p;        /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i2;          /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i3;          /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i4;          /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T temporalCounter_i5;          /* '<S9>/SuperB2_DEGB1_0' */
  uint8_T is_active_c4_SupervisorB2;   /* '<Root>/B1_Failure_Fallback' */
  uint8_T is_c4_SupervisorB2;          /* '<Root>/B1_Failure_Fallback' */
  boolean_T degraded_sup;              /* '<Root>/Monitor Board Status1' */
  boolean_T degraded_rx;               /* '<Root>/Monitor Board Status1' */
  boolean_T degraded_imu;              /* '<Root>/Monitor Board Status1' */
  boolean_T degraded_esp;              /* '<Root>/Monitor Board Status1' */
  boolean_T last_valid_prev_not_empty; /* '<Root>/Monitor Board Status1' */
  boolean_T init_not_empty;            /* '<Root>/Imu-odometry coherence' */
  boolean_T done180;                   /* '<S4>/Check rover safety state1' */
  boolean_T roverIsStopped;            /* '<S4>/Check rover safety state1' */
  boolean_T is_rotating180;            /* '<S4>/Check rover safety state1' */
  boolean_T invert_y;                  /* '<S4>/Check rover safety state1' */
  boolean_T start180;                  /* '<S4>/Check rover safety state1' */
  boolean_T estop_recover_enabled;     /* '<S4>/Check rover safety state1' */
} DW_SupervisorB2_T;

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
  ImuCoherenceStatus_t imu_coherence_status;/* '<Root>/imu_coherence_status' */
  real32_T acc_imu;                    /* '<Root>/acc_imu' */
  real32_T acc_enc;                    /* '<Root>/acc_enc' */
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
 * '<S1>'   : 'SupervisorB2/B1_Failure_Fallback'
 * '<S2>'   : 'SupervisorB2/Board 2 fault masks'
 * '<S3>'   : 'SupervisorB2/Generate Decision for B1'
 * '<S4>'   : 'SupervisorB2/Generate speed ref for b1 fallback'
 * '<S5>'   : 'SupervisorB2/Imu-odometry coherence'
 * '<S6>'   : 'SupervisorB2/MATLAB Function'
 * '<S7>'   : 'SupervisorB2/MATLAB Function1'
 * '<S8>'   : 'SupervisorB2/Monitor Board Status1'
 * '<S9>'   : 'SupervisorB2/Generate Decision for B1/superB2_DEGB1_0'
 * '<S10>'  : 'SupervisorB2/Generate Decision for B1/superB2_DEGB1_0/SuperB2_DEGB1_0'
 * '<S11>'  : 'SupervisorB2/Generate speed ref for b1 fallback/Check rover safety state'
 * '<S12>'  : 'SupervisorB2/Generate speed ref for b1 fallback/Check rover safety state1'
 * '<S13>'  : 'SupervisorB2/Generate speed ref for b1 fallback/Generate Fallback Decision'
 * '<S14>'  : 'SupervisorB2/Generate speed ref for b1 fallback/Generate Fallback Decision/superB2_DEGB1_1'
 * '<S15>'  : 'SupervisorB2/Generate speed ref for b1 fallback/Generate Fallback Decision/superB2_DEGB1_1/SuperB2_DEGB1_1'
 */
#endif                                 /* SupervisorB2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SupervisorB1.c
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

#include "SupervisorB1.h"
#include "SupervisorB1_types.h"
#include "rtwtypes.h"
#include "snapshot/board_health_snapshot.h"
#include "shared_headers/supervisor_command.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <string.h>
#include "SupervisorB1_private.h"

/* Named constants for Chart: '<S1>/Check rover safety state' */
#define Supervi_IN_Rover_Critical_State ((uint8_T)1U)
#define Supervi_IN_Rover_Degraded_State ((uint8_T)2U)
#define SupervisorB1_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define SupervisorB_IN_Rover_Safe_State ((uint8_T)3U)

/* Named constants for Chart: '<S1>/Rover motion state' */
#define Sup_IN_Procedura_emergency_stop ((uint8_T)1U)
#define Supe_IN_In_frenata_di_emergenza ((uint8_T)1U)
#define Superv_IN_Combo_toggle_backward ((uint8_T)2U)
#define Supervi_IN_Combo_starting_point ((uint8_T)1U)
#define Supervis_IN_Schivata_a_sinistra ((uint8_T)6U)
#define Superviso_IN_Stato_marcia_rover ((uint8_T)2U)
#define SupervisorB1_IN_Evita_destra   ((uint8_T)1U)
#define SupervisorB1_IN_Evita_sinistra ((uint8_T)2U)
#define SupervisorB1_IN_Motori_fermi   ((uint8_T)2U)
#define SupervisorB1_IN_Normale_marcia ((uint8_T)3U)
#define SupervisorB1_IN_Retromarcia_180 ((uint8_T)4U)
#define SupervisorB1_IN_Step2          ((uint8_T)1U)
#define SupervisorB1_IN_Step3          ((uint8_T)2U)
#define SupervisorB1_IN_Step4          ((uint8_T)3U)
#define Supervisor_IN_Schivata_a_destra ((uint8_T)5U)

/* Named constants for Chart: '<Root>/Decide actuation privileges' */
#define SupervisorB1_IN_B1_actuating   ((uint8_T)1U)
#define SupervisorB1_IN_B2_actuating   ((uint8_T)2U)

/* Named constants for Chart: '<Root>/Monitor Board Local Status' */
#define IN_FrontRight_encoder_critical ((uint8_T)1U)
#define S_IN_FrontLeft_encoder_critical ((uint8_T)1U)
#define S_IN_RearRight_encoder_critical ((uint8_T)1U)
#define Su_IN_FrontRight_motor_critical ((uint8_T)2U)
#define Su_IN_RearLeft_encoder_critical ((uint8_T)1U)
#define Sup_IN_FrontLeft_motor_critical ((uint8_T)2U)
#define Sup_IN_RearRight_motor_critical ((uint8_T)2U)
#define Supe_IN_RearLeft_motor_critical ((uint8_T)2U)
#define Supervi_IN_Temperature_critical ((uint8_T)1U)
#define Supervi_IN_Temperature_degraded ((uint8_T)2U)
#define Supervis_IN_Temperature_timeout ((uint8_T)4U)
#define SupervisorB1_IN_B2_sup_critical ((uint8_T)1U)
#define SupervisorB1_IN_B2_sup_degraded ((uint8_T)2U)
#define SupervisorB1_IN_B2_sup_ok      ((uint8_T)3U)
#define SupervisorB1_IN_Battery_ok     ((uint8_T)3U)
#define SupervisorB1_IN_Battery_timeout ((uint8_T)4U)
#define SupervisorB1_IN_FrontLeft_ok   ((uint8_T)3U)
#define SupervisorB1_IN_FrontRight_ok  ((uint8_T)3U)
#define SupervisorB1_IN_RearLeft_ok    ((uint8_T)3U)
#define SupervisorB1_IN_RearRight_ok   ((uint8_T)3U)
#define SupervisorB1_IN_Rx_critical    ((uint8_T)1U)
#define SupervisorB1_IN_Rx_degraded    ((uint8_T)2U)
#define SupervisorB1_IN_Rx_ok          ((uint8_T)3U)
#define SupervisorB1_IN_Temperature_ok ((uint8_T)3U)
#define SupervisorB_IN_Battery_critical ((uint8_T)1U)
#define SupervisorB_IN_Battery_degraded ((uint8_T)2U)

/* Block signals (default storage) */
B_SupervisorB1_T SupervisorB1_B;

/* Block states (default storage) */
DW_SupervisorB1_T SupervisorB1_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_SupervisorB1_T SupervisorB1_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_SupervisorB1_T SupervisorB1_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_SupervisorB1_T SupervisorB1_Y;

/* Real-time model */
static RT_MODEL_SupervisorB1_T SupervisorB1_M_;
RT_MODEL_SupervisorB1_T *const SupervisorB1_M = &SupervisorB1_M_;

/* Forward declaration for local functions */
static boolean_T SupervisorB1_checkTempTimeout(uint32_T now_ms, uint32_T
  temp_last_valid_ms, real32_T temperature_degC);
static void SupervisorB1_MonitorTemperature(const BoardHealthSnapshot_t
  *BusConversion_InsertedFor_Monit, TemperatureStatus *temp_status);
static boolean_T SupervisorB_checkBatteryTimeout(uint32_T now_ms, uint32_T
  batt_last_valid_ms, real32_T batt_pct_last);
static void SupervisorB1_MonitorBattery(const BoardHealthSnapshot_t
  *BusConversion_InsertedFor_Monit, BatteryStatus *battery_status);
static void SupervisorB1_MonitorWheels(void);
static boolean_T Supe_isCommDegradedByMeanPeriod(uint32_T data_last_valid_ms,
  real_T mean_threshold_ms);
static void SupervisorB1_updateSafetyLimits(SafetyStatus rover_state, real_T
  *V_MAX, real_T *OMEGA_MAX, real_T *V_MAX_MANEUVER, real_T *OMEGA_GO_LEFT,
  real_T *OMEGA_GO_RIGHT);
static real32_T SupervisorB1_angleCalculator180(real32_T gyro_angle);
static real32_T SupervisorB1_angleError(real32_T gyro_angle, real32_T
  targetAngle);
static boolean_T SupervisorB1_checkStop(const real32_T wheel_speeds[4]);
static void enter_atomic_Stato_marcia_rover(const SafetyStatus
  *rover_safety_state);
static void SupervisorB1_Motion_Supervision(const SafetyStatus
  *rover_safety_state);
static boolean_T resultZC0;
static boolean_T resultZC1;
static boolean_T resultZC2;
static boolean_T resultZC3;
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static boolean_T SupervisorB1_checkTempTimeout(uint32_T now_ms, uint32_T
  temp_last_valid_ms, real32_T temperature_degC)
{
  uint32_T dt_s;
  dt_s = now_ms -
    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
    /*MW:OvSatOk*/ temp_last_valid_ms;
  if (dt_s > now_ms) {
    dt_s = 0U;
  }

  dt_s = (uint32_T)rt_roundd_snf((real_T)dt_s / 1000.0);
  return (!(dt_s < 0.5)) && (temperature_degC + (real32_T)dt_s >= 65.0F);
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static void SupervisorB1_MonitorTemperature(const BoardHealthSnapshot_t
  *BusConversion_InsertedFor_Monit, TemperatureStatus *temp_status)
{
  switch (SupervisorB1_DW.is_MonitorTemperature) {
   case Supervi_IN_Temperature_critical:
    *temp_status = TEMP_HEALTH_CRITICAL;
    if ((!(BusConversion_InsertedFor_Monit->temperature_degC < 50.0F)) &&
        (!(BusConversion_InsertedFor_Monit->temperature_degC > 0.0F))) {
      SupervisorB1_DW.durationCounter_1_kv = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_kv > 250U) {
      SupervisorB1_DW.durationCounter_1_a = 0U;
      SupervisorB1_DW.is_MonitorTemperature = SupervisorB1_IN_Temperature_ok;
      *temp_status = TEMP_HEALTH_OK;
    }
    break;

   case Supervi_IN_Temperature_degraded:
    *temp_status = TEMP_HEALTH_DEGRADED;
    if ((!(BusConversion_InsertedFor_Monit->temperature_degC < 50.0F)) &&
        (!(BusConversion_InsertedFor_Monit->temperature_degC > 0.0F))) {
      SupervisorB1_DW.durationCounter_1_k = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_k > 500U) {
      SupervisorB1_DW.durationCounter_1_a = 0U;
      SupervisorB1_DW.is_MonitorTemperature = SupervisorB1_IN_Temperature_ok;
      *temp_status = TEMP_HEALTH_OK;
    } else {
      if ((!(BusConversion_InsertedFor_Monit->temperature_degC > 65.0F)) &&
          (!(BusConversion_InsertedFor_Monit->temperature_degC < -15.0F))) {
        SupervisorB1_DW.durationCounter_2 = 0U;
      }

      if (SupervisorB1_DW.durationCounter_2 > 200U) {
        SupervisorB1_DW.durationCounter_1_kv = 0U;
        SupervisorB1_DW.is_MonitorTemperature = Supervi_IN_Temperature_critical;
        *temp_status = TEMP_HEALTH_CRITICAL;
      } else if (SupervisorB1_checkTempTimeout(SupervisorB1_U.now_ms,
                  BusConversion_InsertedFor_Monit->temp_last_valid_ms,
                  BusConversion_InsertedFor_Monit->temperature_degC)) {
        SupervisorB1_DW.durationCounter_1 = 0U;
        SupervisorB1_DW.is_MonitorTemperature = Supervis_IN_Temperature_timeout;
        *temp_status = TEMP_HEALTH_CRITICAL;
      }
    }
    break;

   case SupervisorB1_IN_Temperature_ok:
    *temp_status = TEMP_HEALTH_OK;
    if ((!(BusConversion_InsertedFor_Monit->temperature_degC > 55.0F)) &&
        (!(BusConversion_InsertedFor_Monit->temperature_degC < -5.0F))) {
      SupervisorB1_DW.durationCounter_1_a = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_a > 500U) {
      SupervisorB1_DW.durationCounter_2 = 0U;
      SupervisorB1_DW.durationCounter_1_k = 0U;
      SupervisorB1_DW.is_MonitorTemperature = Supervi_IN_Temperature_degraded;
      *temp_status = TEMP_HEALTH_DEGRADED;
    } else if (SupervisorB1_checkTempTimeout(SupervisorB1_U.now_ms,
                BusConversion_InsertedFor_Monit->temp_last_valid_ms,
                BusConversion_InsertedFor_Monit->temperature_degC)) {
      SupervisorB1_DW.durationCounter_1 = 0U;
      SupervisorB1_DW.is_MonitorTemperature = Supervis_IN_Temperature_timeout;
      *temp_status = TEMP_HEALTH_CRITICAL;
    }
    break;

   default:
    /* case IN_Temperature_timeout: */
    *temp_status = TEMP_HEALTH_CRITICAL;
    if ((!(BusConversion_InsertedFor_Monit->temperature_degC < 50.0F)) &&
        (!(BusConversion_InsertedFor_Monit->temperature_degC > 0.0F))) {
      SupervisorB1_DW.durationCounter_1 = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1 > 250U) {
      SupervisorB1_DW.durationCounter_1_a = 0U;
      SupervisorB1_DW.is_MonitorTemperature = SupervisorB1_IN_Temperature_ok;
      *temp_status = TEMP_HEALTH_OK;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static boolean_T SupervisorB_checkBatteryTimeout(uint32_T now_ms, uint32_T
  batt_last_valid_ms, real32_T batt_pct_last)
{
  uint32_T dt_s;
  boolean_T timeout_fault;
  dt_s = now_ms -
    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
    /*MW:OvSatOk*/ batt_last_valid_ms;
  if (dt_s > now_ms) {
    dt_s = 0U;
  }

  dt_s = (uint32_T)rt_roundd_snf((real_T)dt_s / 1000.0);
  if (dt_s < 0.5) {
    timeout_fault = false;
  } else {
    timeout_fault = (batt_pct_last - (real32_T)((uint32_T)rt_roundd_snf
      (rt_roundd_snf((real_T)dt_s / 3600.0) * 50.0 / 3.3) * 100U) <= 23.0F);
  }

  return timeout_fault;
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static void SupervisorB1_MonitorBattery(const BoardHealthSnapshot_t
  *BusConversion_InsertedFor_Monit, BatteryStatus *battery_status)
{
  switch (SupervisorB1_DW.is_MonitorBattery) {
   case SupervisorB_IN_Battery_critical:
    *battery_status = BATTERY_HEALTH_CRITICAL;
    if (!(BusConversion_InsertedFor_Monit->battery_pct > 25.0F)) {
      SupervisorB1_DW.durationCounter_1_i = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_i > 250U) {
      SupervisorB1_DW.durationCounter_1_l = 0U;
      SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_ok;
      *battery_status = BATTERY_HEALTH_OK;
    }
    break;

   case SupervisorB_IN_Battery_degraded:
    *battery_status = BATTERY_HEALTH_DEGRADED;
    if (!(BusConversion_InsertedFor_Monit->battery_pct > 25.0F)) {
      SupervisorB1_DW.durationCounter_1_m = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_m > 250U) {
      SupervisorB1_DW.durationCounter_1_l = 0U;
      SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_ok;
      *battery_status = BATTERY_HEALTH_OK;
    } else {
      if (!(BusConversion_InsertedFor_Monit->battery_pct < 15.0F)) {
        SupervisorB1_DW.durationCounter_2_f = 0U;
      }

      if (SupervisorB1_DW.durationCounter_2_f > 250U) {
        SupervisorB1_DW.durationCounter_1_i = 0U;
        SupervisorB1_DW.is_MonitorBattery = SupervisorB_IN_Battery_critical;
        *battery_status = BATTERY_HEALTH_CRITICAL;
      } else if (SupervisorB_checkBatteryTimeout(SupervisorB1_U.now_ms,
                  BusConversion_InsertedFor_Monit->batt_last_valid_ms,
                  BusConversion_InsertedFor_Monit->battery_pct)) {
        SupervisorB1_DW.durationCounter_1_e = 0U;
        SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_timeout;
        *battery_status = BATTERY_HEALTH_CRITICAL;
      }
    }
    break;

   case SupervisorB1_IN_Battery_ok:
    *battery_status = BATTERY_HEALTH_OK;
    if (!(BusConversion_InsertedFor_Monit->battery_pct < 23.0F)) {
      SupervisorB1_DW.durationCounter_1_l = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_l > 250U) {
      SupervisorB1_DW.durationCounter_2_f = 0U;
      SupervisorB1_DW.durationCounter_1_m = 0U;
      SupervisorB1_DW.is_MonitorBattery = SupervisorB_IN_Battery_degraded;
      *battery_status = BATTERY_HEALTH_DEGRADED;
    } else if (SupervisorB_checkBatteryTimeout(SupervisorB1_U.now_ms,
                BusConversion_InsertedFor_Monit->batt_last_valid_ms,
                BusConversion_InsertedFor_Monit->battery_pct)) {
      SupervisorB1_DW.durationCounter_1_e = 0U;
      SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_timeout;
      *battery_status = BATTERY_HEALTH_CRITICAL;
    }
    break;

   default:
    /* case IN_Battery_timeout: */
    *battery_status = BATTERY_HEALTH_CRITICAL;
    if (!(BusConversion_InsertedFor_Monit->battery_pct > 25.0F)) {
      SupervisorB1_DW.durationCounter_1_e = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_e > 250U) {
      SupervisorB1_DW.durationCounter_1_l = 0U;
      SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_ok;
      *battery_status = BATTERY_HEALTH_OK;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static void SupervisorB1_MonitorWheels(void)
{
  switch (SupervisorB1_DW.is_MonitorFrontLeft) {
   case S_IN_FrontLeft_encoder_critical:
    /* Inport: '<Root>/Encoder' incorporates:
     *  Inport: '<Root>/Board2_Data'
     */
    if (!SupervisorB1_U.Encoder.hasNoFeedback[0]) {
      SupervisorB1_DW.is_MonitorFrontLeft = SupervisorB1_IN_FrontLeft_ok;
      SupervisorB1_B.wheel_status[0] = WHEEL_OK;
    } else if ((!SupervisorB1_U.Board2_Data.payload.isMotionConsistent) &&
               (SupervisorB1_B.rx_status != RX_CRITICAL)) {
      SupervisorB1_DW.is_MonitorFrontLeft = Sup_IN_FrontLeft_motor_critical;
      SupervisorB1_B.wheel_status[0] = WHEEL_CRITICAL_MOTOR;
    }
    break;

   case Sup_IN_FrontLeft_motor_critical:
    break;

   default:
    /* Inport: '<Root>/Encoder' */
    /* case IN_FrontLeft_ok: */
    if (SupervisorB1_U.Encoder.hasNoFeedback[0]) {
      SupervisorB1_DW.is_MonitorFrontLeft = S_IN_FrontLeft_encoder_critical;
      SupervisorB1_B.wheel_status[0] = WHEEL_DEGRADED_ENCODER;
    }
    break;
  }

  switch (SupervisorB1_DW.is_MonitorFrontRight) {
   case IN_FrontRight_encoder_critical:
    /* Inport: '<Root>/Encoder' incorporates:
     *  Inport: '<Root>/Board2_Data'
     */
    if (!SupervisorB1_U.Encoder.hasNoFeedback[1]) {
      SupervisorB1_DW.is_MonitorFrontRight = SupervisorB1_IN_FrontRight_ok;
      SupervisorB1_B.wheel_status[1] = WHEEL_OK;
    } else if ((!SupervisorB1_U.Board2_Data.payload.isMotionConsistent) &&
               (SupervisorB1_B.rx_status != RX_CRITICAL)) {
      SupervisorB1_DW.is_MonitorFrontRight = Su_IN_FrontRight_motor_critical;
      SupervisorB1_B.wheel_status[1] = WHEEL_CRITICAL_MOTOR;
    }
    break;

   case Su_IN_FrontRight_motor_critical:
    break;

   default:
    /* Inport: '<Root>/Encoder' */
    /* case IN_FrontRight_ok: */
    if (SupervisorB1_U.Encoder.hasNoFeedback[1]) {
      SupervisorB1_DW.is_MonitorFrontRight = IN_FrontRight_encoder_critical;
      SupervisorB1_B.wheel_status[1] = WHEEL_DEGRADED_ENCODER;
    }
    break;
  }

  switch (SupervisorB1_DW.is_MonitorRearLeft) {
   case Su_IN_RearLeft_encoder_critical:
    /* Inport: '<Root>/Encoder' incorporates:
     *  Inport: '<Root>/Board2_Data'
     */
    if (!SupervisorB1_U.Encoder.hasNoFeedback[2]) {
      SupervisorB1_DW.is_MonitorRearLeft = SupervisorB1_IN_RearLeft_ok;
      SupervisorB1_B.wheel_status[2] = WHEEL_OK;
    } else if ((!SupervisorB1_U.Board2_Data.payload.isMotionConsistent) &&
               (SupervisorB1_B.rx_status != RX_CRITICAL)) {
      SupervisorB1_DW.is_MonitorRearLeft = Supe_IN_RearLeft_motor_critical;
      SupervisorB1_B.wheel_status[2] = WHEEL_CRITICAL_MOTOR;
    }
    break;

   case Supe_IN_RearLeft_motor_critical:
    break;

   default:
    /* Inport: '<Root>/Encoder' */
    /* case IN_RearLeft_ok: */
    if (SupervisorB1_U.Encoder.hasNoFeedback[2]) {
      SupervisorB1_DW.is_MonitorRearLeft = Su_IN_RearLeft_encoder_critical;
      SupervisorB1_B.wheel_status[2] = WHEEL_DEGRADED_ENCODER;
    }
    break;
  }

  switch (SupervisorB1_DW.is_MonitorRearRight) {
   case S_IN_RearRight_encoder_critical:
    /* Inport: '<Root>/Encoder' incorporates:
     *  Inport: '<Root>/Board2_Data'
     */
    if (!SupervisorB1_U.Encoder.hasNoFeedback[3]) {
      SupervisorB1_DW.is_MonitorRearRight = SupervisorB1_IN_RearRight_ok;
      SupervisorB1_B.wheel_status[3] = WHEEL_OK;
    } else if ((!SupervisorB1_U.Board2_Data.payload.isMotionConsistent) &&
               (SupervisorB1_B.rx_status != RX_CRITICAL)) {
      SupervisorB1_DW.is_MonitorRearRight = Sup_IN_RearRight_motor_critical;
      SupervisorB1_B.wheel_status[3] = WHEEL_CRITICAL_MOTOR;
    }
    break;

   case Sup_IN_RearRight_motor_critical:
    break;

   default:
    /* Inport: '<Root>/Encoder' */
    /* case IN_RearRight_ok: */
    if (SupervisorB1_U.Encoder.hasNoFeedback[3]) {
      SupervisorB1_DW.is_MonitorRearRight = S_IN_RearRight_encoder_critical;
      SupervisorB1_B.wheel_status[3] = WHEEL_DEGRADED_ENCODER;
    }
    break;
  }
}

/* Function for Chart: '<Root>/Monitor Board Local Status' */
static boolean_T Supe_isCommDegradedByMeanPeriod(uint32_T data_last_valid_ms,
  real_T mean_threshold_ms)
{
  real_T old;
  uint32_T mean_dt;
  uint32_T q0;
  boolean_T comm_degraded;
  if (!SupervisorB1_DW.last_valid_prev_not_empty) {
    SupervisorB1_DW.last_valid_prev = data_last_valid_ms;
    SupervisorB1_DW.last_valid_prev_not_empty = true;
    memset(&SupervisorB1_DW.buf[0], 0, 10U * sizeof(real_T));
    SupervisorB1_DW.idx = 1.0;
    SupervisorB1_DW.count = 0.0;
    SupervisorB1_DW.sum_dt = 0U;
    comm_degraded = false;
  } else {
    if (data_last_valid_ms != SupervisorB1_DW.last_valid_prev) {
      mean_dt = data_last_valid_ms -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ SupervisorB1_DW.last_valid_prev;
      if (mean_dt > data_last_valid_ms) {
        mean_dt = 0U;
      }

      SupervisorB1_DW.last_valid_prev = data_last_valid_ms;
      old = SupervisorB1_DW.buf[(int32_T)SupervisorB1_DW.idx - 1];
      SupervisorB1_DW.buf[(int32_T)SupervisorB1_DW.idx - 1] = mean_dt;
      old = rt_roundd_snf((real_T)SupervisorB1_DW.sum_dt - old);
      if (old < 4.294967296E+9) {
        if (old >= 0.0) {
          q0 = (uint32_T)old;
        } else {
          q0 = 0U;
        }
      } else {
        q0 = MAX_uint32_T;
      }

      SupervisorB1_DW.sum_dt = q0 + /*MW:OvSatOk*/ mean_dt;
      if (SupervisorB1_DW.sum_dt < q0) {
        SupervisorB1_DW.sum_dt = MAX_uint32_T;
      }

      SupervisorB1_DW.idx++;
      if (SupervisorB1_DW.idx > 10.0) {
        SupervisorB1_DW.idx = 1.0;
      }

      if (SupervisorB1_DW.count < 10.0) {
        SupervisorB1_DW.count++;
      }
    }

    if (SupervisorB1_DW.count > 0.0) {
      old = rt_roundd_snf((real_T)SupervisorB1_DW.sum_dt / SupervisorB1_DW.count);
      if (old < 4.294967296E+9) {
        mean_dt = (uint32_T)old;
      } else {
        mean_dt = MAX_uint32_T;
      }
    } else {
      mean_dt = 0U;
    }

    comm_degraded = (mean_dt > mean_threshold_ms);
  }

  return comm_degraded;
}

/* Function for Chart: '<S1>/Rover motion state' */
static void SupervisorB1_updateSafetyLimits(SafetyStatus rover_state, real_T
  *V_MAX, real_T *OMEGA_MAX, real_T *V_MAX_MANEUVER, real_T *OMEGA_GO_LEFT,
  real_T *OMEGA_GO_RIGHT)
{
  *V_MAX = 1.0;
  *OMEGA_MAX = 1.0;
  *V_MAX_MANEUVER = 0.8;
  *OMEGA_GO_LEFT = 0.4;
  *OMEGA_GO_RIGHT = -0.4;
  switch (rover_state) {
   case SAFETY_DEGRADED:
    *V_MAX = 0.5;
    *OMEGA_MAX = 0.5;
    *V_MAX_MANEUVER = 0.4;
    *OMEGA_GO_LEFT = 0.2;
    *OMEGA_GO_RIGHT = -0.2;
    break;

   case SAFETY_CRITICAL:
    *V_MAX = 0.0;
    *OMEGA_MAX = 0.0;
    *V_MAX_MANEUVER = 0.0;
    *OMEGA_GO_LEFT = 0.0;
    *OMEGA_GO_RIGHT = 0.0;
    break;
  }
}

/* Function for Chart: '<S1>/Rover motion state' */
static real32_T SupervisorB1_angleCalculator180(real32_T gyro_angle)
{
  real32_T targetAngle;
  if (gyro_angle + 180.0F > 360.0F) {
    targetAngle = (gyro_angle + 180.0F) - 360.0F;
  } else {
    targetAngle = gyro_angle + 180.0F;
  }

  return targetAngle;
}

/* Function for Chart: '<S1>/Rover motion state' */
static real32_T SupervisorB1_angleError(real32_T gyro_angle, real32_T
  targetAngle)
{
  real32_T x;
  x = (targetAngle - gyro_angle) + 180.0F;
  if (rtIsNaNF(x)) {
    x = (rtNaNF);
  } else if (rtIsInfF(x)) {
    x = (rtNaNF);
  } else if (x == 0.0F) {
    x = 0.0F;
  } else {
    x = fmodf(x, 360.0F);
    if (x == 0.0F) {
      x = 0.0F;
    } else if (x < 0.0F) {
      x += 360.0F;
    }
  }

  return x - 180.0F;
}

/* Function for Chart: '<S1>/Rover motion state' */
static boolean_T SupervisorB1_checkStop(const real32_T wheel_speeds[4])
{
  int32_T b_k;
  boolean_T x[4];
  boolean_T exitg1;
  boolean_T stop;
  x[0] = (fabsf(wheel_speeds[0]) < 2.5F);
  x[1] = (fabsf(wheel_speeds[1]) < 2.5F);
  x[2] = (fabsf(wheel_speeds[2]) < 2.5F);
  x[3] = (fabsf(wheel_speeds[3]) < 2.5F);
  stop = true;
  b_k = 0;
  exitg1 = false;
  while ((!exitg1) && (b_k < 4)) {
    if (!x[b_k]) {
      stop = false;
      exitg1 = true;
    } else {
      b_k++;
    }
  }

  return stop;
}

/* Function for Chart: '<S1>/Rover motion state' */
static void enter_atomic_Stato_marcia_rover(const SafetyStatus
  *rover_safety_state)
{
  real_T OMEGA_GO_LEFT;
  real_T OMEGA_GO_RIGHT;
  real_T OMEGA_MAX;
  real_T V_MAX;
  real_T V_MAX_MANEUVER;
  SupervisorB1_updateSafetyLimits(*rover_safety_state, &V_MAX, &OMEGA_MAX,
    &V_MAX_MANEUVER, &OMEGA_GO_LEFT, &OMEGA_GO_RIGHT);

  /* Inport: '<Root>/Board2_Data' */
  SupervisorB1_DW.v_user = SupervisorB1_U.Board2_Data.payload.y_norm * (real32_T)
    V_MAX;
  if (SupervisorB1_DW.invert_y) {
    if (SupervisorB1_DW.v_user >= 0.0F) {
      SupervisorB1_DW.invert_y = false;
    } else {
      SupervisorB1_DW.v_user = -SupervisorB1_DW.v_user;
    }
  }
}

/* Function for Chart: '<S1>/Rover motion state' */
static void SupervisorB1_Motion_Supervision(const SafetyStatus
  *rover_safety_state)
{
  real_T OMEGA_GO_LEFT;
  real_T OMEGA_GO_RIGHT;
  real_T OMEGA_MAX;
  real_T V_MAX;
  real_T V_MAX_MANEUVER;
  real32_T omega_user;
  real32_T tmp;
  if (SupervisorB1_DW.is_Motion_Supervision == Sup_IN_Procedura_emergency_stop)
  {
    if ((*rover_safety_state != SAFETY_CRITICAL) &&
        SupervisorB1_DW.estop_recover_enabled) {
      /* Outport: '<Root>/actuate_emergency_stop' */
      SupervisorB1_Y.actuate_emergency_stop = false;
      SupervisorB1_DW.is_Procedura_emergency_stop =
        SupervisorB1_IN_NO_ACTIVE_CHILD;
      SupervisorB1_DW.is_Motion_Supervision = Superviso_IN_Stato_marcia_rover;
      enter_atomic_Stato_marcia_rover(rover_safety_state);
      SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_NORMAL;
    } else if (SupervisorB1_DW.is_Procedura_emergency_stop ==
               Supe_IN_In_frenata_di_emergenza) {
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_ESTOP;
      if (!SupervisorB1_DW.roverIsStopped) {
        SupervisorB1_DW.durationCounter_1_m0 = 0U;
      }

      if (SupervisorB1_DW.durationCounter_1_m0 > 50U) {
        SupervisorB1_DW.is_Procedura_emergency_stop =
          SupervisorB1_IN_Motori_fermi;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_STOP;
        SupervisorB1_DW.estop_recover_enabled = true;
      } else {
        /* Inport: '<Root>/Encoder' */
        SupervisorB1_DW.roverIsStopped = SupervisorB1_checkStop
          (SupervisorB1_U.Encoder.wheel_speed_rpm);
        if (!SupervisorB1_DW.roverIsStopped) {
          SupervisorB1_DW.durationCounter_1_m0 = 0U;
        }
      }
    } else {
      /* Outport: '<Root>/current_action' */
      /* case IN_Motori_fermi: */
      SupervisorB1_Y.current_action = CMD_STOP;
    }

    /* Inport: '<Root>/Board2_Data' */
    /* case IN_Stato_marcia_rover: */
  } else if (((SupervisorB1_U.Board2_Data.payload.command == CMD_ESTOP) &&
              (SupervisorB1_DW.v_user > 0.0F) &&
              (!SupervisorB1_DW.is_rotating180)) || (*rover_safety_state ==
              SAFETY_CRITICAL)) {
    SupervisorB1_DW.estop_recover_enabled = false;
    if (SupervisorB1_DW.is_Stato_marcia_rover == SupervisorB1_IN_Retromarcia_180)
    {
      SupervisorB1_DW.is_rotating180 = false;
      SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_NO_ACTIVE_CHILD;
    } else {
      SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_NO_ACTIVE_CHILD;
    }

    SupervisorB1_DW.invert_y = false;
    SupervisorB1_DW.is_Motion_Supervision = Sup_IN_Procedura_emergency_stop;
    SupervisorB1_DW.durationCounter_1_m0 = 0U;
    SupervisorB1_DW.is_Procedura_emergency_stop =
      Supe_IN_In_frenata_di_emergenza;

    /* Outport: '<Root>/current_action' */
    SupervisorB1_Y.current_action = CMD_ESTOP;

    /* Outport: '<Root>/v_ref' */
    SupervisorB1_Y.v_ref = 0.0F;

    /* Outport: '<Root>/omega_ref' */
    SupervisorB1_Y.omega_ref = 0.0F;

    /* Outport: '<Root>/actuate_emergency_stop' */
    SupervisorB1_Y.actuate_emergency_stop = true;
  } else {
    SupervisorB1_updateSafetyLimits(*rover_safety_state, &V_MAX, &OMEGA_MAX,
      &V_MAX_MANEUVER, &OMEGA_GO_LEFT, &OMEGA_GO_RIGHT);
    SupervisorB1_DW.v_user = SupervisorB1_U.Board2_Data.payload.y_norm *
      (real32_T)V_MAX;
    omega_user = SupervisorB1_U.Board2_Data.payload.x_norm * (real32_T)OMEGA_MAX;
    if (SupervisorB1_DW.invert_y) {
      if (SupervisorB1_DW.v_user >= 0.0F) {
        SupervisorB1_DW.invert_y = false;
      } else {
        SupervisorB1_DW.v_user = -SupervisorB1_DW.v_user;
      }
    }

    switch (SupervisorB1_DW.is_Stato_marcia_rover) {
     case SupervisorB1_IN_Evita_destra:
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_AVOID_RIGHT;
      if (SupervisorB1_U.Board2_Data.payload.command == CMD_NORMAL) {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_NORMAL;
      } else {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = SupervisorB1_DW.v_user;

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = fmaxf(omega_user, 0.0F);
      }
      break;

     case SupervisorB1_IN_Evita_sinistra:
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_AVOID_LEFT;
      if (SupervisorB1_U.Board2_Data.payload.command == CMD_NORMAL) {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_NORMAL;
      } else {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = SupervisorB1_DW.v_user;

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = fminf(omega_user, 0.0F);
      }
      break;

     case SupervisorB1_IN_Normale_marcia:
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_NORMAL;
      if ((SupervisorB1_DW.v_user > 0.0F) &&
          (SupervisorB1_U.Board2_Data.payload.command == CMD_GO_LEFT)) {
        SupervisorB1_DW.is_Stato_marcia_rover = Supervis_IN_Schivata_a_sinistra;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_GO_LEFT;
      } else if ((SupervisorB1_DW.v_user > 0.0F) &&
                 (SupervisorB1_U.Board2_Data.payload.command == CMD_AVOID_LEFT))
      {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Evita_sinistra;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_AVOID_LEFT;
      } else if ((SupervisorB1_DW.v_user > 0.0F) &&
                 (SupervisorB1_U.Board2_Data.payload.command == CMD_AVOID_RIGHT))
      {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Evita_destra;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_AVOID_RIGHT;
      } else if ((SupervisorB1_DW.v_user > 0.0F) &&
                 (SupervisorB1_U.Board2_Data.payload.command == CMD_GO_RIGHT)) {
        SupervisorB1_DW.is_Stato_marcia_rover = Supervisor_IN_Schivata_a_destra;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_GO_RIGHT;
      } else if ((SupervisorB1_U.Board2_Data.payload.y_norm < -0.5F) &&
                 (!SupervisorB1_DW.backward_enabled)) {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Retromarcia_180;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_ROTATE_180;
        SupervisorB1_DW.is_rotating180 = true;
        SupervisorB1_DW.targetAngle = SupervisorB1_angleCalculator180
          (SupervisorB1_U.Board2_Data.payload.yaw);
        SupervisorB1_DW.angle_err = SupervisorB1_angleError
          (SupervisorB1_U.Board2_Data.payload.yaw, SupervisorB1_DW.targetAngle);
      } else if (SupervisorB1_DW.backward_enabled ||
                 (SupervisorB1_U.Board2_Data.payload.y_norm > 0.0F)) {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = SupervisorB1_DW.v_user;

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = omega_user;
      } else {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = 0.0F;

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = omega_user;
      }
      break;

     case SupervisorB1_IN_Retromarcia_180:
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_ROTATE_180;
      if (fabsf(SupervisorB1_DW.angle_err) < 7.5F) {
        SupervisorB1_DW.invert_y = ((SupervisorB1_DW.v_user < 0.0F) ||
          SupervisorB1_DW.invert_y);
        SupervisorB1_DW.is_rotating180 = false;
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_NORMAL;
      } else {
        SupervisorB1_DW.angle_err = SupervisorB1_angleError
          (SupervisorB1_U.Board2_Data.payload.yaw, SupervisorB1_DW.targetAngle);
        omega_user = fabsf(SupervisorB1_DW.angle_err);
        if (omega_user > 45.0F) {
          if (rtIsNaNF(-SupervisorB1_DW.angle_err)) {
            omega_user = (rtNaNF);
          } else if (-SupervisorB1_DW.angle_err < 0.0F) {
            omega_user = -1.0F;
          } else {
            omega_user = (real32_T)(-SupervisorB1_DW.angle_err > 0.0F);
          }

          /* Outport: '<Root>/omega_ref' */
          SupervisorB1_Y.omega_ref = omega_user * (real32_T)OMEGA_MAX;
        } else if (*rover_safety_state == SAFETY_OK) {
          if (rtIsNaNF(-SupervisorB1_DW.angle_err)) {
            tmp = (rtNaNF);
          } else if (-SupervisorB1_DW.angle_err < 0.0F) {
            tmp = -1.0F;
          } else {
            tmp = (real32_T)(-SupervisorB1_DW.angle_err > 0.0F);
          }

          /* Outport: '<Root>/omega_ref' */
          SupervisorB1_Y.omega_ref = fminf((real32_T)OMEGA_MAX, fmaxf(omega_user,
            15.0F) * 0.005F) * tmp;
        }
      }
      break;

     case Supervisor_IN_Schivata_a_destra:
      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_GO_RIGHT;
      if (SupervisorB1_U.Board2_Data.payload.command == CMD_NORMAL) {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_NORMAL;
      } else {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = fminf(SupervisorB1_DW.v_user, (real32_T)
          V_MAX_MANEUVER);

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = fminf(omega_user, (real32_T)OMEGA_GO_RIGHT);
      }
      break;

     default:
      /* Outport: '<Root>/current_action' */
      /* case IN_Schivata_a_sinistra: */
      SupervisorB1_Y.current_action = CMD_GO_LEFT;
      if (SupervisorB1_U.Board2_Data.payload.command == CMD_NORMAL) {
        SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

        /* Outport: '<Root>/current_action' */
        SupervisorB1_Y.current_action = CMD_NORMAL;
      } else {
        /* Outport: '<Root>/v_ref' */
        SupervisorB1_Y.v_ref = fminf(SupervisorB1_DW.v_user, (real32_T)
          V_MAX_MANEUVER);

        /* Outport: '<Root>/omega_ref' */
        SupervisorB1_Y.omega_ref = fmaxf(omega_user, (real32_T)OMEGA_GO_LEFT);
      }
      break;
    }

    /* End of Inport: '<Root>/Board2_Data' */
  }
}

/* Model step function */
void SupervisorB1_step(void)
{
  int32_T crit_mask;
  int32_T degr_mask;
  uint32_T qY;
  boolean_T guard1;
  boolean_T rtb_degraded_required;
  boolean_T rtb_stop_required;
  BatteryStatus battery_status;
  SafetyStatus rover_safety_state;
  SupervisorStatus b2_sup_status;
  TemperatureStatus temp_status;

  /* Chart: '<Root>/Monitor Board Local Status' incorporates:
   *  Inport: '<Root>/Board2_Data'
   *  Inport: '<Root>/Board_Health'
   *  Inport: '<Root>/last_valid_b2_ms'
   *  Inport: '<Root>/now_ms'
   */
  if (SupervisorB1_DW.is_active_c2_SupervisorB1 == 0) {
    SupervisorB1_DW.is_active_c2_SupervisorB1 = 1U;
    SupervisorB1_DW.durationCounter_1_a = 0U;
    SupervisorB1_DW.is_MonitorTemperature = SupervisorB1_IN_Temperature_ok;
    temp_status = TEMP_HEALTH_OK;
    SupervisorB1_DW.durationCounter_1_l = 0U;
    SupervisorB1_DW.is_MonitorBattery = SupervisorB1_IN_Battery_ok;
    battery_status = BATTERY_HEALTH_OK;
    SupervisorB1_DW.is_MonitorFrontLeft = SupervisorB1_IN_FrontLeft_ok;
    SupervisorB1_B.wheel_status[0] = WHEEL_OK;
    SupervisorB1_DW.is_MonitorFrontRight = SupervisorB1_IN_FrontRight_ok;
    SupervisorB1_B.wheel_status[1] = WHEEL_OK;
    SupervisorB1_DW.is_MonitorRearLeft = SupervisorB1_IN_RearLeft_ok;
    SupervisorB1_B.wheel_status[2] = WHEEL_OK;
    SupervisorB1_DW.is_MonitorRearRight = SupervisorB1_IN_RearRight_ok;
    SupervisorB1_B.wheel_status[3] = WHEEL_OK;
    SupervisorB1_DW.is_MonitorBoard2Supervisor = SupervisorB1_IN_B2_sup_ok;
    b2_sup_status = SUPERVISOR_OK;
    SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_ok;
    SupervisorB1_B.rx_status = RX_OK;
  } else {
    SupervisorB1_MonitorTemperature(&SupervisorB1_U.Board_Health, &temp_status);
    SupervisorB1_MonitorBattery(&SupervisorB1_U.Board_Health, &battery_status);
    SupervisorB1_MonitorWheels();
    switch (SupervisorB1_DW.is_MonitorBoard2Supervisor) {
     case SupervisorB1_IN_B2_sup_critical:
      b2_sup_status = SUPERVISOR_CRITICAL;
      if (SupervisorB1_DW.degraded_sup) {
        SupervisorB1_DW.durationCounter_1_h = 0U;
      }

      if (SupervisorB1_DW.durationCounter_1_h > 50U) {
        SupervisorB1_DW.is_MonitorBoard2Supervisor = SupervisorB1_IN_B2_sup_ok;
        b2_sup_status = SUPERVISOR_OK;
      } else {
        SupervisorB1_DW.degraded_sup = Supe_isCommDegradedByMeanPeriod
          (SupervisorB1_U.last_valid_b2_ms, 40.0);
        if (SupervisorB1_DW.degraded_sup) {
          SupervisorB1_DW.durationCounter_1_h = 0U;
          SupervisorB1_DW.durationCounter_1_o = 0U;
        }
      }
      break;

     case SupervisorB1_IN_B2_sup_degraded:
      b2_sup_status = SUPERVISOR_DEGRADED;
      if (SupervisorB1_DW.degraded_sup) {
        SupervisorB1_DW.durationCounter_1_o = 0U;
      }

      if (SupervisorB1_DW.durationCounter_1_o > 50U) {
        SupervisorB1_DW.is_MonitorBoard2Supervisor = SupervisorB1_IN_B2_sup_ok;
        b2_sup_status = SUPERVISOR_OK;
      } else {
        qY = SupervisorB1_U.now_ms -
          /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
          /*MW:OvSatOk*/ SupervisorB1_U.last_valid_b2_ms;
        if (qY > SupervisorB1_U.now_ms) {
          qY = 0U;
        }

        if (qY > 120U) {
          SupervisorB1_DW.durationCounter_1_h = 0U;
          SupervisorB1_DW.is_MonitorBoard2Supervisor =
            SupervisorB1_IN_B2_sup_critical;
          b2_sup_status = SUPERVISOR_CRITICAL;
        } else {
          SupervisorB1_DW.degraded_sup = Supe_isCommDegradedByMeanPeriod
            (SupervisorB1_U.last_valid_b2_ms, 40.0);
          if (SupervisorB1_DW.degraded_sup) {
            SupervisorB1_DW.durationCounter_1_h = 0U;
            SupervisorB1_DW.durationCounter_1_o = 0U;
          }
        }
      }
      break;

     default:
      /* case IN_B2_sup_ok: */
      b2_sup_status = SUPERVISOR_OK;
      if (SupervisorB1_DW.degraded_sup) {
        SupervisorB1_DW.durationCounter_1_o = 0U;
        SupervisorB1_DW.is_MonitorBoard2Supervisor =
          SupervisorB1_IN_B2_sup_degraded;
        b2_sup_status = SUPERVISOR_DEGRADED;
      } else {
        qY = SupervisorB1_U.now_ms -
          /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
          /*MW:OvSatOk*/ SupervisorB1_U.last_valid_b2_ms;
        if (qY > SupervisorB1_U.now_ms) {
          qY = 0U;
        }

        if (qY > 120U) {
          SupervisorB1_DW.durationCounter_1_h = 0U;
          SupervisorB1_DW.is_MonitorBoard2Supervisor =
            SupervisorB1_IN_B2_sup_critical;
          b2_sup_status = SUPERVISOR_CRITICAL;
        } else {
          SupervisorB1_DW.degraded_sup = Supe_isCommDegradedByMeanPeriod
            (SupervisorB1_U.last_valid_b2_ms, 40.0);
          if (SupervisorB1_DW.degraded_sup) {
            SupervisorB1_DW.durationCounter_1_h = 0U;
            SupervisorB1_DW.durationCounter_1_o = 0U;
          }
        }
      }
      break;
    }

    switch (SupervisorB1_DW.is_MonitorRx) {
     case SupervisorB1_IN_Rx_critical:
      SupervisorB1_B.rx_status = RX_CRITICAL;
      if (SupervisorB1_DW.degraded_rx) {
        SupervisorB1_DW.durationCounter_1_p = 0U;
      }

      if (SupervisorB1_DW.durationCounter_1_p > 50U) {
        SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_ok;
        SupervisorB1_B.rx_status = RX_OK;
      } else {
        SupervisorB1_DW.degraded_rx = Supe_isCommDegradedByMeanPeriod
          (SupervisorB1_U.Board2_Data.data_last_valid_ms, 40.0);
        if (SupervisorB1_DW.degraded_rx) {
          SupervisorB1_DW.durationCounter_1_p = 0U;
          SupervisorB1_DW.durationCounter_1_ie = 0U;
        }
      }
      break;

     case SupervisorB1_IN_Rx_degraded:
      SupervisorB1_B.rx_status = RX_DEGRADED;
      if (SupervisorB1_DW.degraded_rx) {
        SupervisorB1_DW.durationCounter_1_ie = 0U;
      }

      if (SupervisorB1_DW.durationCounter_1_ie > 50U) {
        SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_ok;
        SupervisorB1_B.rx_status = RX_OK;
      } else {
        qY = SupervisorB1_U.now_ms -
          /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
          /*MW:OvSatOk*/ SupervisorB1_U.Board2_Data.data_last_valid_ms;
        if (qY > SupervisorB1_U.now_ms) {
          qY = 0U;
        }

        if (qY > 120U) {
          SupervisorB1_DW.durationCounter_1_p = 0U;
          SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_critical;
          SupervisorB1_B.rx_status = RX_CRITICAL;
        } else {
          SupervisorB1_DW.degraded_rx = Supe_isCommDegradedByMeanPeriod
            (SupervisorB1_U.Board2_Data.data_last_valid_ms, 40.0);
          if (SupervisorB1_DW.degraded_rx) {
            SupervisorB1_DW.durationCounter_1_p = 0U;
            SupervisorB1_DW.durationCounter_1_ie = 0U;
          }
        }
      }
      break;

     default:
      /* case IN_Rx_ok: */
      SupervisorB1_B.rx_status = RX_OK;
      if (SupervisorB1_DW.degraded_rx) {
        SupervisorB1_DW.durationCounter_1_ie = 0U;
        SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_degraded;
        SupervisorB1_B.rx_status = RX_DEGRADED;
      } else {
        qY = SupervisorB1_U.now_ms -
          /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
          /*MW:OvSatOk*/ SupervisorB1_U.Board2_Data.data_last_valid_ms;
        if (qY > SupervisorB1_U.now_ms) {
          qY = 0U;
        }

        if (qY > 120U) {
          SupervisorB1_DW.durationCounter_1_p = 0U;
          SupervisorB1_DW.is_MonitorRx = SupervisorB1_IN_Rx_critical;
          SupervisorB1_B.rx_status = RX_CRITICAL;
        } else {
          SupervisorB1_DW.degraded_rx = Supe_isCommDegradedByMeanPeriod
            (SupervisorB1_U.Board2_Data.data_last_valid_ms, 40.0);
          if (SupervisorB1_DW.degraded_rx) {
            SupervisorB1_DW.durationCounter_1_p = 0U;
            SupervisorB1_DW.durationCounter_1_ie = 0U;
          }
        }
      }
      break;
    }
  }

  if ((SupervisorB1_U.Board_Health.temperature_degC < 50.0F) ||
      (SupervisorB1_U.Board_Health.temperature_degC > 0.0F)) {
    SupervisorB1_DW.durationCounter_1++;
    SupervisorB1_DW.durationCounter_1_k++;
    SupervisorB1_DW.durationCounter_1_kv++;
  } else {
    SupervisorB1_DW.durationCounter_1 = 0U;
    SupervisorB1_DW.durationCounter_1_k = 0U;
    SupervisorB1_DW.durationCounter_1_kv = 0U;
  }

  if ((SupervisorB1_U.Board_Health.temperature_degC > 55.0F) ||
      (SupervisorB1_U.Board_Health.temperature_degC < -5.0F)) {
    SupervisorB1_DW.durationCounter_1_a++;
  } else {
    SupervisorB1_DW.durationCounter_1_a = 0U;
  }

  if ((SupervisorB1_U.Board_Health.temperature_degC > 65.0F) ||
      (SupervisorB1_U.Board_Health.temperature_degC < -15.0F)) {
    SupervisorB1_DW.durationCounter_2++;
  } else {
    SupervisorB1_DW.durationCounter_2 = 0U;
  }

  if (SupervisorB1_U.Board_Health.battery_pct > 25.0F) {
    SupervisorB1_DW.durationCounter_1_e++;
    SupervisorB1_DW.durationCounter_1_m++;
  } else {
    SupervisorB1_DW.durationCounter_1_e = 0U;
    SupervisorB1_DW.durationCounter_1_m = 0U;
  }

  if (SupervisorB1_U.Board_Health.battery_pct < 23.0F) {
    SupervisorB1_DW.durationCounter_1_l++;
  } else {
    SupervisorB1_DW.durationCounter_1_l = 0U;
  }

  if (SupervisorB1_U.Board_Health.battery_pct > 25.0F) {
    SupervisorB1_DW.durationCounter_1_i++;
  } else {
    SupervisorB1_DW.durationCounter_1_i = 0U;
  }

  if (SupervisorB1_U.Board_Health.battery_pct < 15.0F) {
    SupervisorB1_DW.durationCounter_2_f++;
  } else {
    SupervisorB1_DW.durationCounter_2_f = 0U;
  }

  if (!SupervisorB1_DW.degraded_sup) {
    SupervisorB1_DW.durationCounter_1_h++;
    SupervisorB1_DW.durationCounter_1_o++;
  } else {
    SupervisorB1_DW.durationCounter_1_h = 0U;
    SupervisorB1_DW.durationCounter_1_o = 0U;
  }

  if (!SupervisorB1_DW.degraded_rx) {
    SupervisorB1_DW.durationCounter_1_p++;
    SupervisorB1_DW.durationCounter_1_ie++;
  } else {
    SupervisorB1_DW.durationCounter_1_p = 0U;
    SupervisorB1_DW.durationCounter_1_ie = 0U;
  }

  /* End of Chart: '<Root>/Monitor Board Local Status' */

  /* MATLAB Function: '<Root>/Board 1 fault masks' */
  crit_mask = 0;
  degr_mask = 0;
  switch (temp_status) {
   case TEMP_HEALTH_CRITICAL:
    crit_mask = 1;
    break;

   case TEMP_HEALTH_DEGRADED:
    degr_mask = 1;
    break;
  }

  switch (battery_status) {
   case BATTERY_HEALTH_CRITICAL:
    crit_mask = (int32_T)((uint32_T)crit_mask | 2U);
    break;

   case BATTERY_HEALTH_DEGRADED:
    degr_mask = (int32_T)((uint32_T)degr_mask | 2U);
    break;
  }

  switch (SupervisorB1_B.rx_status) {
   case RX_CRITICAL:
    crit_mask = (int32_T)((uint32_T)crit_mask | 4U);
    break;

   case RX_DEGRADED:
    degr_mask = (int32_T)((uint32_T)degr_mask | 4U);
    break;
  }

  switch (SupervisorB1_B.wheel_status[0]) {
   case WHEEL_CRITICAL_MOTOR:
    crit_mask = (int32_T)((uint32_T)crit_mask | 8U);
    break;

   case WHEEL_DEGRADED_ENCODER:
    degr_mask = (int32_T)((uint32_T)degr_mask | 8U);
    break;
  }

  switch (SupervisorB1_B.wheel_status[1]) {
   case WHEEL_CRITICAL_MOTOR:
    crit_mask = (int32_T)((uint32_T)crit_mask | 16U);
    break;

   case WHEEL_DEGRADED_ENCODER:
    degr_mask = (int32_T)((uint32_T)degr_mask | 16U);
    break;
  }

  switch (SupervisorB1_B.wheel_status[2]) {
   case WHEEL_CRITICAL_MOTOR:
    crit_mask = (int32_T)((uint32_T)crit_mask | 32U);
    break;

   case WHEEL_DEGRADED_ENCODER:
    degr_mask = (int32_T)((uint32_T)degr_mask | 32U);
    break;
  }

  switch (SupervisorB1_B.wheel_status[3]) {
   case WHEEL_CRITICAL_MOTOR:
    crit_mask = (int32_T)((uint32_T)crit_mask | 64U);
    break;

   case WHEEL_DEGRADED_ENCODER:
    degr_mask = (int32_T)((uint32_T)degr_mask | 64U);
    break;
  }

  switch (b2_sup_status) {
   case SUPERVISOR_CRITICAL:
    crit_mask = (int32_T)((uint32_T)crit_mask | 128U);
    break;

   case SUPERVISOR_DEGRADED:
    degr_mask = (int32_T)((uint32_T)degr_mask | 128U);
    break;
  }

  /* Outport: '<Root>/critical_mask' incorporates:
   *  MATLAB Function: '<Root>/Board 1 fault masks'
   */
  SupervisorB1_Y.critical_mask = (uint32_T)crit_mask;

  /* Outport: '<Root>/degraded_mask' incorporates:
   *  MATLAB Function: '<Root>/Board 1 fault masks'
   */
  SupervisorB1_Y.degraded_mask = (uint32_T)degr_mask;

  /* MATLAB Function: '<Root>/Process B2 masks' incorporates:
   *  Inport: '<Root>/Board2_Data'
   */
  rtb_stop_required = ((SupervisorB1_U.Board2_Data.payload.critical_mask & 3U)
                       != 0U);
  rtb_degraded_required = (((SupervisorB1_U.Board2_Data.payload.critical_mask &
    4U) != 0U) || ((SupervisorB1_U.Board2_Data.payload.critical_mask & 8U) != 0U)
    || (SupervisorB1_U.Board2_Data.payload.degraded_mask != 0U));

  /* Chart: '<Root>/Decide actuation privileges' */
  if (SupervisorB1_DW.is_active_c4_SupervisorB1 == 0) {
    SupervisorB1_DW.is_active_c4_SupervisorB1 = 1U;
    SupervisorB1_DW.is_c4_SupervisorB1 = SupervisorB1_IN_B1_actuating;
    SupervisorB1_Y.give_b2_actuation = false;
  } else if (SupervisorB1_DW.is_c4_SupervisorB1 == SupervisorB1_IN_B1_actuating)
  {
    SupervisorB1_Y.give_b2_actuation = false;
    if ((SupervisorB1_B.rx_status == RX_CRITICAL) || (b2_sup_status ==
         SUPERVISOR_CRITICAL)) {
      SupervisorB1_DW.durationCounter_1_d = 0U;
      SupervisorB1_DW.is_c4_SupervisorB1 = SupervisorB1_IN_B2_actuating;
      SupervisorB1_Y.give_b2_actuation = true;
    }
  } else {
    /* case IN_B2_actuating: */
    SupervisorB1_Y.give_b2_actuation = true;
    if ((SupervisorB1_B.rx_status == RX_CRITICAL) || (b2_sup_status ==
         SUPERVISOR_CRITICAL)) {
      SupervisorB1_DW.durationCounter_1_d = 0U;
    }

    if (SupervisorB1_DW.durationCounter_1_d > 150U) {
      SupervisorB1_DW.is_c4_SupervisorB1 = SupervisorB1_IN_B1_actuating;
      SupervisorB1_Y.give_b2_actuation = false;
    }
  }

  if ((SupervisorB1_B.rx_status != RX_CRITICAL) && (b2_sup_status !=
       SUPERVISOR_CRITICAL)) {
    SupervisorB1_DW.durationCounter_1_d++;
  } else {
    SupervisorB1_DW.durationCounter_1_d = 0U;
  }

  /* End of Chart: '<Root>/Decide actuation privileges' */

  /* Outputs for Enabled SubSystem: '<Root>/Actuation decisions and motion state' incorporates:
   *  EnablePort: '<S1>/Enable'
   */
  /* Logic: '<Root>/Logical Operator' */
  if (!SupervisorB1_Y.give_b2_actuation) {
    /* Chart: '<S1>/Check rover safety state' incorporates:
     *  MATLAB Function: '<Root>/Board 1 fault masks'
     */
    if (SupervisorB1_DW.is_active_c7_SupervisorB1 == 0) {
      SupervisorB1_DW.is_active_c7_SupervisorB1 = 1U;
      SupervisorB1_DW.is_c7_SupervisorB1 = SupervisorB_IN_Rover_Safe_State;
      rover_safety_state = SAFETY_OK;
    } else {
      switch (SupervisorB1_DW.is_c7_SupervisorB1) {
       case Supervi_IN_Rover_Critical_State:
        rover_safety_state = SAFETY_CRITICAL;
        if ((crit_mask == 0) && (!rtb_stop_required)) {
          if ((degr_mask == 0) && (!rtb_degraded_required)) {
            SupervisorB1_DW.is_c7_SupervisorB1 = SupervisorB_IN_Rover_Safe_State;
            rover_safety_state = SAFETY_OK;
          } else if ((degr_mask != 0) || rtb_degraded_required) {
            SupervisorB1_DW.is_c7_SupervisorB1 = Supervi_IN_Rover_Degraded_State;
            rover_safety_state = SAFETY_DEGRADED;
          }
        }
        break;

       case Supervi_IN_Rover_Degraded_State:
        rover_safety_state = SAFETY_DEGRADED;
        if ((degr_mask == 0) && (!rtb_degraded_required)) {
          SupervisorB1_DW.is_c7_SupervisorB1 = SupervisorB_IN_Rover_Safe_State;
          rover_safety_state = SAFETY_OK;
        } else if ((crit_mask != 0) || rtb_stop_required) {
          SupervisorB1_DW.is_c7_SupervisorB1 = Supervi_IN_Rover_Critical_State;
          rover_safety_state = SAFETY_CRITICAL;
        }
        break;

       default:
        /* case IN_Rover_Safe_State: */
        rover_safety_state = SAFETY_OK;
        if ((crit_mask != 0) || rtb_stop_required) {
          SupervisorB1_DW.is_c7_SupervisorB1 = Supervi_IN_Rover_Critical_State;
          rover_safety_state = SAFETY_CRITICAL;
        } else if ((degr_mask != 0) || rtb_degraded_required) {
          SupervisorB1_DW.is_c7_SupervisorB1 = Supervi_IN_Rover_Degraded_State;
          rover_safety_state = SAFETY_DEGRADED;
        }
        break;
      }
    }

    /* End of Chart: '<S1>/Check rover safety state' */

    /* Chart: '<S1>/Rover motion state' incorporates:
     *  Inport: '<Root>/Board2_Data'
     */
    if (SupervisorB1_DW.temporalCounter_i1 < 63) {
      SupervisorB1_DW.temporalCounter_i1++;
    }

    resultZC3 = ((int32_T)rt_ZCFcn(RISING_ZERO_CROSSING,
      &SupervisorB1_DW.previousZC_k,
      ((real_T)SupervisorB1_U.Board2_Data.payload.btn2)) != 0);
    resultZC2 = ((int32_T)rt_ZCFcn(RISING_ZERO_CROSSING,
      &SupervisorB1_DW.previousZC_j,
      ((real_T)SupervisorB1_U.Board2_Data.payload.btn1)) != 0);
    resultZC1 = ((int32_T)rt_ZCFcn(RISING_ZERO_CROSSING,
      &SupervisorB1_DW.previousZC_n,
      ((real_T)SupervisorB1_U.Board2_Data.payload.btn2)) != 0);
    resultZC0 = ((int32_T)rt_ZCFcn(RISING_ZERO_CROSSING,
      &SupervisorB1_DW.previousZC,
      ((real_T)SupervisorB1_U.Board2_Data.payload.btn1)) != 0);
    if (SupervisorB1_DW.is_active_c3_SupervisorB1 == 0) {
      SupervisorB1_DW.is_active_c3_SupervisorB1 = 1U;
      SupervisorB1_DW.backward_enabled = false;
      SupervisorB1_DW.is_Backward_mode_toggle = Supervi_IN_Combo_starting_point;
      SupervisorB1_DW.is_Motion_Supervision = Superviso_IN_Stato_marcia_rover;
      enter_atomic_Stato_marcia_rover(&rover_safety_state);
      SupervisorB1_DW.is_Stato_marcia_rover = SupervisorB1_IN_Normale_marcia;

      /* Outport: '<Root>/current_action' */
      SupervisorB1_Y.current_action = CMD_NORMAL;
    } else {
      if (SupervisorB1_DW.is_Backward_mode_toggle ==
          Supervi_IN_Combo_starting_point) {
        if (resultZC2 && (SupervisorB1_U.Board2_Data.payload.x_norm == 0.0F) &&
            (SupervisorB1_U.Board2_Data.payload.y_norm == 0.0F)) {
          SupervisorB1_DW.is_Backward_mode_toggle =
            Superv_IN_Combo_toggle_backward;
          SupervisorB1_DW.temporalCounter_i1 = 0U;
          SupervisorB1_DW.is_Combo_toggle_backward = SupervisorB1_IN_Step2;
        }

        /* case IN_Combo_toggle_backward: */
      } else if ((SupervisorB1_U.Board2_Data.payload.x_norm != 0.0F) ||
                 (SupervisorB1_U.Board2_Data.payload.y_norm != 0.0F)) {
        SupervisorB1_DW.is_Combo_toggle_backward =
          SupervisorB1_IN_NO_ACTIVE_CHILD;
        SupervisorB1_DW.is_Backward_mode_toggle =
          Supervi_IN_Combo_starting_point;
      } else {
        guard1 = false;
        switch (SupervisorB1_DW.is_Combo_toggle_backward) {
         case SupervisorB1_IN_Step2:
          if (resultZC0) {
            SupervisorB1_DW.temporalCounter_i1 = 0U;
            SupervisorB1_DW.is_Combo_toggle_backward = SupervisorB1_IN_Step3;
          } else if (SupervisorB1_DW.temporalCounter_i1 >= 50) {
            SupervisorB1_DW.is_Combo_toggle_backward =
              SupervisorB1_IN_NO_ACTIVE_CHILD;
            SupervisorB1_DW.is_Backward_mode_toggle =
              Supervi_IN_Combo_starting_point;
          }
          break;

         case SupervisorB1_IN_Step3:
          if (resultZC1) {
            SupervisorB1_DW.temporalCounter_i1 = 0U;
            SupervisorB1_DW.is_Combo_toggle_backward = SupervisorB1_IN_Step4;
          } else if (SupervisorB1_DW.temporalCounter_i1 >= 50) {
            SupervisorB1_DW.is_Combo_toggle_backward =
              SupervisorB1_IN_NO_ACTIVE_CHILD;
            SupervisorB1_DW.is_Backward_mode_toggle =
              Supervi_IN_Combo_starting_point;
          }
          break;

         default:
          /* case IN_Step4: */
          if (resultZC3) {
            SupervisorB1_DW.backward_enabled = !SupervisorB1_DW.backward_enabled;
            guard1 = true;
          } else if (SupervisorB1_DW.temporalCounter_i1 >= 50) {
            guard1 = true;
          }
          break;
        }

        if (guard1) {
          SupervisorB1_DW.is_Combo_toggle_backward =
            SupervisorB1_IN_NO_ACTIVE_CHILD;
          SupervisorB1_DW.is_Backward_mode_toggle =
            Supervi_IN_Combo_starting_point;
        }
      }

      SupervisorB1_Motion_Supervision(&rover_safety_state);
    }

    if (SupervisorB1_DW.roverIsStopped) {
      SupervisorB1_DW.durationCounter_1_m0++;
    } else {
      SupervisorB1_DW.durationCounter_1_m0 = 0U;
    }

    /* End of Chart: '<S1>/Rover motion state' */
  }

  /* End of Logic: '<Root>/Logical Operator' */
  /* End of Outputs for SubSystem: '<Root>/Actuation decisions and motion state' */
}

/* Model initialize function */
void SupervisorB1_initialize(void)
{
  SupervisorB1_PrevZCX.SFunction_edgeDetectionSignal_Z = UNINITIALIZED_ZCSIG;
  SupervisorB1_PrevZCX.SFunction_edgeDetectionSignal_j = UNINITIALIZED_ZCSIG;
  SupervisorB1_PrevZCX.SFunction_edgeDetectionSigna_jl = UNINITIALIZED_ZCSIG;
  SupervisorB1_PrevZCX.SFunction_edgeDetectionSign_jls = UNINITIALIZED_ZCSIG;

  /* SystemInitialize for Enabled SubSystem: '<Root>/Actuation decisions and motion state' */
  /* SystemInitialize for Chart: '<S1>/Rover motion state' */
  SupervisorB1_DW.previousZC = 3U;
  SupervisorB1_DW.previousZC_n = 3U;
  SupervisorB1_DW.previousZC_j = 3U;
  SupervisorB1_DW.previousZC_k = 3U;

  /* End of SystemInitialize for SubSystem: '<Root>/Actuation decisions and motion state' */
}

/* Model terminate function */
void SupervisorB1_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

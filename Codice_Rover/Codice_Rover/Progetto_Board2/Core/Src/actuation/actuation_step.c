/*
 * actuation_task.c
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */


#include "actuation/actuation.h"
#include "cmsis_os2.h"
#include <math.h>
#include <stdio.h>

#define MAX_SPEED_RPM 160.0f
#define MAX_SPEED_DEGRADED_RPM 80.0f
#define MAX_DELTA_RPM 4.0f
#define MAX_VOLTAGE 12.0f

static float rate_limit(float target, float prev, float max_delta)
{
    float delta = target - prev;

    if (delta > max_delta){
        delta = max_delta;
    }
    else if (delta < -max_delta){
        delta = -max_delta;
    }

    return prev + delta;
}



void Actuation_Init(void)
{
    Sabertooth_Init();
}


/* Step di attuazione board 2 (open loop) */
void Actuation_Step(float v_ref, float omega_ref)
{

	static float u_left_rpm_last = 0;
	static float u_right_rpm_last = 0;

	float v_rpm = v_ref * MAX_SPEED_RPM;
	float omega_rpm = omega_ref * MAX_SPEED_RPM;

	/* lato sinistro */
	float u_left_rpm  = v_rpm - omega_rpm;

	if (u_left_rpm > MAX_SPEED_DEGRADED_RPM) u_left_rpm = MAX_SPEED_DEGRADED_RPM;
	if (u_left_rpm < -MAX_SPEED_DEGRADED_RPM) u_left_rpm  = -MAX_SPEED_DEGRADED_RPM;

	/* lato destro */
	float u_right_rpm = v_rpm + omega_rpm;

	if (u_right_rpm > MAX_SPEED_DEGRADED_RPM) u_right_rpm = MAX_SPEED_DEGRADED_RPM;
    if (u_right_rpm < -MAX_SPEED_DEGRADED_RPM) u_right_rpm  = -MAX_SPEED_DEGRADED_RPM;

    /* === Rate limiting (per accellerazione smooth) === */
	u_left_rpm = rate_limit(u_left_rpm, u_left_rpm_last, MAX_DELTA_RPM);
	u_right_rpm = rate_limit(u_right_rpm, u_right_rpm_last, MAX_DELTA_RPM);

	u_left_rpm_last = u_left_rpm;
	u_right_rpm_last = u_right_rpm;

    /* Scaling rpm -> voltaggio */
    float u_left_v = MAX_VOLTAGE * u_left_rpm / MAX_SPEED_RPM;
    float u_right_v = MAX_VOLTAGE * u_right_rpm / MAX_SPEED_RPM;


    /* === Attuazione HW come su board 1=== */
    Sabertooth_ApplyOutputs(u_left_v, u_right_v, u_left_v, u_right_v);
}

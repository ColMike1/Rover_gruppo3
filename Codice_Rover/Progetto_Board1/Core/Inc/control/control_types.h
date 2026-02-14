/*
 * control_types.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_CONTROL_CONTROL_TYPES_H_
#define INC_CONTROL_CONTROL_TYPES_H_

#include <stdint.h>

/* ===== Input della control law ===== */
typedef struct
{
    /* Riferimenti fisici decisi dalla supervisione */
    float speed_ref_rpm;   /* velocità longitudinale */
    float steering_cmd;    /* comando di sterzata */

    /* Velocità ruote misurate [RPM] */
    float rpm_sx_p;
    float rpm_dx_p;
    float rpm_sx_a;
    float rpm_dx_a;

} ControlInput_t;


/* ===== Output della control law ===== */
typedef struct
{
    /* Comandi ruote [RPM] */
    float u_sx_a;
    float u_dx_a;
    float u_sx_p;
    float u_dx_p;

} ControlOutput_t;

#endif /* INC_CONTROL_CONTROL_TYPES_H_ */

/*
 * control_law.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_CONTROL_CONTROL_LAW_H_
#define INC_CONTROL_CONTROL_LAW_H_

#include <stdint.h>
#include "control_types.h"


/* Inizializzazione della control law
   (reset PID, stati interni, ecc.) */
void ControlLaw_Init(void);

/* Step della control law
   - in  : input del controllo
   - out : output del controllo */
void ControlLaw_Step(const ControlInput_t *in,
                     ControlOutput_t *out);

float ControlLaw_SideSpeed(float speed_ref_rpm, float rpm_side_avg);

#endif /* INC_CONTROL_CONTROL_LAW_H_ */

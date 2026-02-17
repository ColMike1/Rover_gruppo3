/*
 * actuation_step.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_ACTUATION_ACTUATION_STEP_H_
#define INC_ACTUATION_ACTUATION_STEP_H_

#include "control_law.h"
void Actuation_Init(void);
void Actuation_Step(ControlOutput_t ctrl_out);

#endif /* INC_ACTUATION_ACTUATION_STEP_H_ */

/*
 * control_step.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_CONTROL_CONTROL_STEP_H_
#define INC_CONTROL_CONTROL_STEP_H_

#include "control/control_types.h"

void Control_Init(void);
ControlOutput_t Control_Step(void);

#endif /* INC_CONTROL_CONTROL_STEP_H_ */

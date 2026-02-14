/*
 * actuation_step.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Sterm
 */

#ifndef INC_ACTUATION_ACTUATION_STEP_H_
#define INC_ACTUATION_ACTUATION_STEP_H_

void Actuation_Init(void);
void Actuation_Step(float v_ref, float omega_ref);

#endif /* INC_ACTUATION_ACTUATION_STEP_H_ */

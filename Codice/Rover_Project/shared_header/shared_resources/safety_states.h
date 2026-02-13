/*
 * safety_states.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef SAFETY_STATES_H_
#define SAFETY_STATES_H_

typedef enum
{
  SAFETY_SAFE = 0,     /* marcia consentita */
  SAFETY_DEGRADED,    /* marcia limitata */
  SAFETY_STOP,        /* stop controllato */
  SAFETY_ESTOP        /* emergenza immediata */
} SafetyState_t;

#endif /* SAFETY_STATES_H_ */

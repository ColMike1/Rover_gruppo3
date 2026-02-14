/*
 * supervisor_command.h
 *
 *  Created on: Jan 18, 2026
 *      Author: Sterm
 */

#ifndef SUPERVISOR_COMMAND_H_
#define SUPERVISOR_COMMAND_H_

typedef enum
{
  CMD_NORMAL = 0,
  CMD_ROTATE_180,
  CMD_GO_LEFT,
  CMD_GO_RIGHT,
  CMD_AVOID_RIGHT,
  CMD_AVOID_LEFT,
  CMD_STOP,
  CMD_ESTOP
} SupervisorCommand_t;

#endif /* SUPERVISOR_COMMAND_H_ */

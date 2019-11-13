#ifndef __StepMotorRege_H__
#define __StepMotorRege_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Regent.h"





VOID StepZAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepSYNAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepZAxisMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepSYNAxisMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepZAxisMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed);
VOID StepSYNAxisMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed);


#endif

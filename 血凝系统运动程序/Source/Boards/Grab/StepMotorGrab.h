#ifndef __StepMotorGrab_H__
#define __StepMotorGrab_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Grab.h"





VOID StepZAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepZAxisMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepZAxisMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed);


#endif

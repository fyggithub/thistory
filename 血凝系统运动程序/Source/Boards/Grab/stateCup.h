#ifndef __stateCup_H__
#define __stateCup_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "stateReady.h"
#include "stateReady.h"
#include "StepMotorGrab.h"
enum{STATE1_CATCHCUP = 0,STATE1_PUTCUP,STATE1_CUP_MAX};

VOID CatchCup(Message* msg);
VOID PutCup(Message* msg);

#endif



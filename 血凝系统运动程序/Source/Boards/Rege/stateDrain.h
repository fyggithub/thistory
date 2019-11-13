#ifndef __stateDrain_H__
#define __stateDrain_H__

#include "Console.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "StepMotor.h"
#include "Regent.h"
#include "SendResultCode.h"
#include "ConfigReage.h"
#include "StepMotorRege.h"
enum {
	STATE1_DRA_BASIC = 0, STATE1_DRA_DOWN1, STATE1_DRA_DRA,
	STATE1_DRA_DOWN2,	  STATE1_DRA_WAIT1, STATE1_DRA_WAIT2,
	STATE1_DRA_UP,		  STATE1_DRA_MAX,
};

VOID RegeStateDraMessageHandler(Message* msg);


#endif


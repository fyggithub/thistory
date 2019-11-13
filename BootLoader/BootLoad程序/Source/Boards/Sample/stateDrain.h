#ifndef __stateDrain_H__
#define __stateDrain_H__

#include "Console.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "StepMotor.h"
#include "Sample.h"
#include "SendResultCode.h"
#include "ConfigSample.h"
#include "StepMotorSample.h"

enum {
	STATE1_DRA_BASIC = 0, STATE1_DRA_DOWN, STATE1_DRA_WAIT,
	STATE1_DRA_UP,		  STATE1_DRA_MAX,
};

VOID SampleStateDraMessageHandler(Message* msg);

#endif




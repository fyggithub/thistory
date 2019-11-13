#ifndef __stateClean_H__
#define __stateClean_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "SendResultCode.h"
#include "Sample.h"
#include "ConfigSample.h"
#include "StepMotorSample.h"

enum {
	STATE1_CLN_BASIC = 0, STATE1_CLN_DOWN, STATE1_CLN_CLN, 
	STATE1_CLN_UP, 		  STATE1_CLN_MAX, 
};

enum {
	STATE2_CLN_CLN_DRA = 0,	STATE2_CLN_CLN_SUB,
	STATE2_CLN_CLN_WAIT, 	STATE2_CLN_CLN_MAX,
};

VOID SampleStateClnMessageHandler(Message* msg);


#endif




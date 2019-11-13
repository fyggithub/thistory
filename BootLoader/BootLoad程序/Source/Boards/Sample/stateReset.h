#ifndef __stateReset_H__
#define __stateReset_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Sample.h"
#include "SendResultCode.h"
#include "ConfigSample.h"

//复位函数中的主状态
enum{STATE1_OPT_INIT = 0,   STATE1_OPT,	 
	 STATE1_NOOPT,			STATE1_OPT_MAX
};
enum{STATE2_OPT_NOOPT_INIT = 0,	STATE2_OPT_NOOPT_LEAVE, 
	 STATE2_OPT_NOOPT_AGENT, 	STATE2_OPT_NOOPT_MAX
};
enum{STATE2_OPT_OPT_INIT = 0,  	STATE2_OPT_OPT_LEAVE,   
	 STATE2_OPT_OPT_AGENT,   	STATE2_OPT_OPT_MAX
};


VOID SampleStateMotorResetMessageHandler(Message* msg);

#endif




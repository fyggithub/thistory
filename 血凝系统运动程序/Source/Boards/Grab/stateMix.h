#ifndef __stateMix_H__
#define __stateMix_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Grab.h"
#include "Pwm.h"
#include "Time.h"
#include "stateMoveWork.h"
#include "stateReady.h"
#include "Gpio.h"
#include "StepMotorGrab.h"
enum{ZAXIS1_BASIC = 0 , WAIT_MIX,
	 CLOSE_MIX,	        ZAXIS1_RISE,
	 MIX1_START,		MIX1_PROCESS,
	 ZAXIS1_DOWN,		ZAXIS1_MAX
};

//1 - 混匀设置为带有加减速  0 - 混匀设置为固定PWM
#define OPEN_PWM_ADJUST        0

#define MIX_START_PWMVALUE      212
#define MIXUP_TIME_LENGTH    	400            
#define MIXDOWN_TIME_LENGTH 	400
#define MIXUP_TIME				100
#define MIXDOWN_TIME			100

VOID MixMotorCtrl(Message *msg);
VOID MixMotor_Contral(Message* msg);
VOID ZAxisMixMovement(Message* msg);
VOID XYMovementMix(Message *msg);



#endif


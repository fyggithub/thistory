#ifndef __Sample_H__
#define __Sample_H__

/********************************************************************
 *
 *******************************************************************/
#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "string.h"

#include "stateReset.h"
#include "stateReady.h"
#include "stateSub.h"
#include "stateDrain.h"
#include "stateMoveWork.h"
#include "stateClean.h"
/********************************************************************
 *
 *******************************************************************/
#define VERSION_XX	1
#define VERSION_YY	0
#define VERSION_ZZ	3

enum{
	STATE0_INIT=0, STATE0_RESET, 	STATE0_READY, 
	STATE0_BASIC,  STATE0_ZAIXDET,  STATE0_MV,		
	STATE0_SUBBUF, STATE0_SUB,      STATE0_DRA,		
	STATE0_DRABUF, STATE0_CLN,	    STATE0_DELAY,	
	STATE0_MAX
};

typedef struct _Instruction{
	UINT16 motor;
	UINT16 motorState;
	UINT16 speed;
	INT32  plusesNum;
	UINT16 mvDir;
	UINT16 aimPluse;
	UINT16 wrkCode;
	UINT16 detState;
	INT32 liquidVolume;

	UINT16 timeCtr;
	UINT16 cntType;
	UINT16 subType;
	UINT16 downType;
	INT32 mgnVolume;
	UINT16 cmpType;
	UINT16 cupVolume;
	UINT16 clnType;
	UINT16 innerTime;
	UINT16 outerTime;
	UINT16 soakTime;

	UINT16 receiveCmd;
	INT32 drianPulse;
	INT16 liquidState;
	INT16 airFlag;
	INT16 interruptFlag;
	INT32 zLowestPulse;
	
	UINT8 count;
	UINT8 postcount;
	UINT8 resetCount[4];
	
	INT16 codeID1;
	INT16 codeID2;
	INT16 codeID3;
	INT16 codeID4;
}StateInstruction;

typedef struct _mainState{
	UINT16 stateManageTop;
	UINT16 stateManageMid;
	UINT16 stateManageBtm;
	UINT16 stateManageEnd;
	StateInstruction Parameter;
	StepMotor motors[4];
	Gpio* gpio;
}mainState;

VOID SampleInit(VOID);

extern mainState MainState;

#endif



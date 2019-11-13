#ifndef __Grab_H__
#define __Grab_H__

/********************************************************************
 *
 *******************************************************************/
#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "string.h"
#include "stateReady.h"
#include "stateReset.h"
#include "stateMoveWork.h"
/********************************************************************
 *
 *******************************************************************/
#define VERSION_XX	1
#define VERSION_YY	0
#define VERSION_ZZ	3

enum{
	STATE0_INIT=0,  STATE0_RESET,    STATE0_READY, 
	STATE0_BASIC ,  STATE0_MIX_CTRL, STATE0_MV   , 
	STATE0_ZAXISPOS,STATE0_CATCH ,   STATE0_PUT  ,   
	STATE0_MIX_TIME,STATE0_MV_MIX,   STATE0_THROW,   
	STATE0_DELAY,   STATE0_MAX
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
	UINT16 timeCtr;
	UINT16 cntType;

	UINT16 alignhole;
	UINT16 wrkPos;
	UINT16 mixstate;
	UINT16 mixstrength;
	UINT16 mixtime;
	UINT16 mixtype;
	UINT16 mixpwm;
	UINT16 throwCupCode;
	
	UINT16 mixMaxPwm;
	UINT16 receiveCmd;
	UINT8 resetCount[4];
	UINT8 count;
	UINT8 savecount1;
	UINT8 savecount2;
	UINT8 stepcount;
	UINT8 count1;
	UINT8 mvover;
	
	
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

VOID GrabInit(VOID);

extern mainState MainState;






#endif




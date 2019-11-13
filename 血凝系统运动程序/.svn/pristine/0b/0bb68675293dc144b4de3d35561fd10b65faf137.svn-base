/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Load.h
 *******************************************************************/
#ifndef _LOAD_H_
#define _LOAD_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
#include "StepMotor.h"
#include "Message.h"
#include "HostLoad.h"
//#include "stateReady.h"
//#define LOAD_TEST_AUTOLOAD

enum{ MOTOR_DISP=0, MOTOR_LOAD, MOTOR_MAX };

enum{
	STATE0_INIT, STATE0_RESET, STATE0_READY, STATE0_BASIC,
	STATE0_MAX,
};

enum{
	STATE1_RST_IDLE,
	STATE1_RST_LEAVE_OPTO, STATE1_RST_MEET_OPTO,
	STATE1_RST_GOTO_OPTO, STATE1_RST_GOTO_RDY,
	STATE1_RST_MAX,
};

enum{
	STATE1_RDY_LOAD_IDLE,
	STATE1_RDY_LOAD_UP, STATE1_RDY_LOAD_UPWAIT,
	STATE1_RDY_LOAD_DOWN, STATE1_RDY_LOAD_DOWNWAIT,
	STATE1_RDY_LOAD_MAX,
};

enum{
	STATE1_RDY_DISP_IDLE,
	STATE1_RDY_DISP_DISP, STATE1_RDY_DISP_MAX,
};

#define MSG_MOTOR_DISP	(1)
#define MSG_MOTOR_LOAD	(2)

#define DISP_ROUND_STEPS	(3200)
#define DISP_QUARTER_STEPS	(800)

#define HOLE_MIN	(1)
#define HOLE_MAX	(4)
#define WORK_MIN	(2)
#define WORK_MAX	(5)

#define LOAD_SPEED_MAX 	(10)

/********************************************************************
 *
 *******************************************************************/
typedef struct _LoadApp{
	UINT8  tag;
	UINT8  state0;
	UINT8  state1;
	UINT8  state1_Load;
	UINT8  state1_Disp;
	UINT8  state_TextLoad;
	UINT8  currentResetMotor;
	UINT32 stateForBack;
	BOOL   bDispWatchSlide;
	BOOL   bAutoLoad;
	UINT16 autoLoadCnt;
	UINT8  whichHoleOnWork;
	UINT8  whichWorkHoleOn;
	BOOL      bMotorReset[MOTOR_MAX];
	StepMotor motors[MOTOR_MAX];
	BOOL   needResponse;
	
	UINT16 receiveCmd;
	UINT16 codeID1;
	UINT16 codeID2;
	UINT16 codeID3;
	UINT16 codeID4;
}LoadApp;

extern LoadApp loadApp;
/********************************************************************
 *
 *******************************************************************/
VOID LoadInit(VOID);
VOID LoadBasicCommandOnStateAndBackTo(UINT32 stateBackTo, Message* msg);
VOID LoadMotorResetStart(UINT16 id);
VOID LoadStepMotorMovAbs(StepMotor* motor, INT32 step, UINT32 speed);
INT32 LoadGetWrkPosByHole1(INT32 work);

VOID ResponseState(VOID);
VOID ResponseSuccess(VOID);
VOID SendParameterMessage(VOID);

VOID MessageOnStateReady(Message* msg);
VOID MessageOnStateReset(Message* msg);

VOID MessageReset(Message* msg);

/********************************************************************
 * End of File
 *******************************************************************/
#endif // _LOAD_H_



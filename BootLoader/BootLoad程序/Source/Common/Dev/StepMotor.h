/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * StepMotor.h
 *******************************************************************/
#ifndef _STEP_MOTOR_H_
#define _STEP_MOTOR_H_
/********************************************************************
 * StepMotor struct
 *******************************************************************/
#include "Datatypes.h"
#include "Gpio.h"
#include "Pulse.h"

#include "AccelerateTable.h"

enum{ MOTOR_STATE_READY = 0, MOTOR_STATE_RUNNING,};

#define MOTOR_SPEED_STAGE_MAX	(16*2-1)
typedef struct _StepMotorSpeedPlan{
	UINT16 freqs[MOTOR_SPEED_STAGE_MAX];
	UINT16 steps[MOTOR_SPEED_STAGE_MAX];
	UINT16 stageSteps;
	UINT16 stage;
	UINT16 stageTotal;
	CONST AccTbl* accTable;
}StepMotorSpeedPlan;

typedef struct _StepMotorOptoCondition{
	BOOL  enableOptoJudge;
	INT16 stateInit;
	INT16 stateChangeCount;
	INT16 setChangeOptTimes;
	INT16 meetOptTimes;
	INT32 posOpto;
}StepMotorOptocondition;

typedef VOID (*StepCallback)(VOID);
typedef struct __StepMotor{
	Gpio*  opto;
	Gpio*  lpd;
	Gpio*  ori;
	Pulse* pulse;
	UINT16 state;    // ready or running
	INT32  pos;	     // startPos when it is running
	INT32  orient;   // 1 or -1;
	INT32  step;     // has moved already
	UINT32 msgReady; // which event will be sent when finished
	UINT16 breakstop;
	
	StepMotorSpeedPlan     speedPlan;
	StepMotorOptocondition optoCond;
	StepCallback  callback;
}StepMotor;
/********************************************************************
 * StepMotor Method
 *******************************************************************/
VOID   StepMotorInit    (StepMotor* motor, UINT32 msgReady);
VOID   StepMotorEnable  (StepMotor* motor);
VOID   StepMotorDisable (StepMotor* motor);
INT32  StepMotorGetPos  (StepMotor* motor);
UINT16 StepMotorGetOpto (StepMotor* motor); // just 1 opto for reset
UINT16 StepMotorGetState(StepMotor* motor);
VOID   StepMotorSetCallback(StepMotor* motor, StepCallback callback);

VOID StepMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed);
VOID StepMotorMovZero(StepMotor* motor, INT32 step, UINT32 speed);
VOID StepMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed);
VOID StepMotorMovUntilOptoTwice(StepMotor* motor, INT32 step, UINT32 speed);
//just call it in callback
VOID StepMotorBrake(StepMotor* motor);

INT32 GetInitOptDistance(StepMotor* motor);
INT32 GetMotorMovPos(StepMotor* motor);



/********************************************************************
 * End of File
 *******************************************************************/
#endif // _STEP_MOTOR_H_

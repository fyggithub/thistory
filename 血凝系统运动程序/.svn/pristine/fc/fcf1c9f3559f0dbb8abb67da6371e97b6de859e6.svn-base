#include "StepMotorGrab.h"

/********************************************************************
 *
 *******************************************************************/
VOID StepZAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed)
{
	if (step < 0){
		MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Z_DOWN);
	}
	else{
		MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Z_UP);
	}
	StepMotorMovRel(motor, step, speed);
}
/********************************************************************
 *
 *******************************************************************/
VOID StepZAxisMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor) {
		INT32 pos = StepMotorGetPos(motor);
		StepZAxisMotorMovRel(motor, (step-pos), speed);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepZAxisMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor && motor->opto){
		motor->optoCond.enableOptoJudge  = TRUE;
		motor->optoCond.stateChangeCount = 0;
		motor->optoCond.stateInit        = GpioGet(motor->opto);
		motor->optoCond.meetOptTimes     = 0;
		motor->optoCond.setChangeOptTimes= 1;
		StepZAxisMotorMovRel(motor, step, speed);
	}
}





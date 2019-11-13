#include "StepMotorRege.h"

/********************************************************************
 *
 *******************************************************************/
VOID StepZAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed)
{
	if (step < 0){
		MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_Z_DOWN);
	}
	else{
		MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_Z_UP);
	}
	StepMotorMovRel(motor, step, speed);
}
/********************************************************************
 *
 *******************************************************************/
VOID StepSYNAxisMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed)
{
	if (step < 0){
		MainState.motors[MOTOR_SYN].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_SYN_DOWN);
	}
	else{
		MainState.motors[MOTOR_SYN].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_SYN_UP);
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
VOID StepSYNAxisMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor) {
		INT32 pos = StepMotorGetPos(motor);
		StepSYNAxisMotorMovRel(motor, (step-pos), speed);
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
/********************************************************************
 *
 *******************************************************************/
VOID StepSYNAxisMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor && motor->opto){
		motor->optoCond.enableOptoJudge  = TRUE;
		motor->optoCond.stateChangeCount = 0;
		motor->optoCond.stateInit        = GpioGet(motor->opto);
		motor->optoCond.meetOptTimes     = 0;
		motor->optoCond.setChangeOptTimes= 1;
		StepSYNAxisMotorMovRel(motor, step, speed);
	}
}




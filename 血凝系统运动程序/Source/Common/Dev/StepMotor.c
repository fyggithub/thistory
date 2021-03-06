/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * StepMotor.c
 *******************************************************************/
#include "StepMotor.h"
#include "Console.h"
/********************************************************************
 *
 *******************************************************************/
#define MOTOR_LPD_ON	(0)
#define MOTOR_LPD_OFF	(1)
#define MOTOR_ORI_FWARD	(0)
#define MOTOR_ORI_BWARD	(1)

/********************************************************************
 *
 *******************************************************************/
STATIC VOID SpeedPlan(StepMotorSpeedPlan* plan, INT32 step, UINT32 speed)
{
	if(plan){
		step = step<0? -step : step;
		plan->stage      = 0;
		plan->stageSteps = 0;
		CONST  AccTbl* acc = plan->accTable;

		if(acc==NULL){
			acc = GetAccelerateTable(0);
			plan->stageTotal = 1;
			plan->steps[0] = step;
			plan->freqs[0] = acc->freqTbl[0];
			return;
		}

		UINT16 stepi = 0,n = 0,k = 0;
		while(n < 15){
			stepi += acc->stepInterval[n] + acc->stepInterval[31-n];
			if(step < stepi){				
				break;
			}
			n++;
		}
		stepi = 0;
		n = n < 1 ? 0 : n;
		n = n > 15? 15: n;
		n = speed < n ? speed : n;
		for(k = 0;k < n;k++){
			stepi += acc->stepInterval[n] + acc->stepInterval[31-n];
		}
		INT32 i = 0;
		for(; i < n; i++){
			plan->steps[i] = acc->stepInterval[i];
			plan->freqs[i] = acc->freqTbl[i];
		}
		plan->steps[n] = step - stepi;
		plan->freqs[n] = acc->freqTbl[n];
		i = n + 1;
		UINT16 j = 32 - n;
		for(;j < 32;j++,i++){
			plan->steps[i] = acc->stepInterval[j];
			plan->freqs[i] = acc->freqTbl[j];
		}
		plan->stageTotal = i;
		
//		UINT16 stepi = acc->stepInterval;
//		UINT16 n = step / (stepi*2);
//		n = n < 1 ? 0 : n-1;
//		n = n > 15? 15: n;
//		n = speed < n ? speed : n;
//		INT32 i = 0;
//		for(; i<n; i++){
//			plan->steps[i] = stepi;
//			plan->freqs[i] = acc->freqTbl[i];
//		}
//		plan->steps[n] = step - (n*2*stepi);
//		plan->freqs[n] = acc->freqTbl[n];
//		i = n+1;
//		INT32 j = n-1;
//		for(; j>=0; i++, j--){
//			plan->steps[i] = stepi;
//			plan->freqs[i] = acc->freqTbl[j];
//		}
//		plan->stageTotal = i;
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID SpeedPlanBrake(StepMotorSpeedPlan* plan)
{
	if(plan){
		UINT16 stage = plan->stage;
		UINT16 stageTotal = plan->stageTotal;
		if(stageTotal <= 2){
			// stop now!!!
			plan->stageTotal = stage;
			return;
		}

		if(stage < stageTotal/2){
			// accelerate
			int i = stage+1;
			int j = stageTotal - stage;
			for(; j<stageTotal; j++, i++){
				plan->steps[i] = plan->steps[j];
				plan->freqs[i] = plan->freqs[j];
			}
			plan->stageTotal = i;
		}
		else if(stage == stageTotal/2){
			// uniform
			plan->steps[stage] = plan->stageSteps;
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL isSpeedPlanDone(StepMotorSpeedPlan* plan)
{
	if(plan){
		if(plan->stage >= plan->stageTotal)
			return TRUE;
	}
	return FALSE;
}

STATIC BOOL SpeedPlanUpdate(StepMotorSpeedPlan* plan)
{
	BOOL needUpdateFreq = FALSE;
	if(plan && !isSpeedPlanDone(plan)){
		plan->stageSteps++;
		if(plan->stageSteps >= plan->steps[plan->stage]){
			plan->stage++;
			plan->stageSteps = 0;
			needUpdateFreq = TRUE;
		}
		if(isSpeedPlanDone(plan))
			needUpdateFreq = FALSE;
	}
	return needUpdateFreq;
}

STATIC UINT32 SpeedPlanFreqGet(StepMotorSpeedPlan* plan)
{
	if(plan && plan->stage < plan->stageTotal){
		return plan->freqs[plan->stage];
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL isOptoChanged(StepMotor* motor)
{
//	if(motor && motor->opto && motor->optoCond.enableOptoJudge){
//		UINT16 opto = GpioGet(motor->opto);
//		if(opto != motor->optoCond.stateInit){
//			motor->optoCond.stateChangeCount++;
//		}
//		if(motor->optoCond.stateChangeCount > 5){
//			return TRUE;
//		}
//	}
//	return FALSE;
	if(motor && motor->opto && motor->optoCond.enableOptoJudge){
		UINT16 opto = GpioGet(motor->opto);
		if(opto != motor->optoCond.stateInit){						
			motor->optoCond.stateChangeCount++;
		}
		
		if(motor->optoCond.stateChangeCount > 5){
			motor->optoCond.stateChangeCount = 0;
			motor->optoCond.meetOptTimes++;
			if(motor->optoCond.meetOptTimes >= motor->optoCond.setChangeOptTimes){   //到达改变光耦的次数 
				motor->optoCond.meetOptTimes = 0;
				return TRUE;
			}
			else{
				motor->optoCond.stateInit = GpioGet(motor->opto);
			}
		}
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MotorPulseCallback(Pulse* pulse)
{
	if(pulse && pulse->owner){
		StepMotor* motor = (StepMotor*)(pulse->owner);
		StepMotorSpeedPlan* plan = &(motor->speedPlan);
		motor->step++;

		if(motor->callback)
			motor->callback();
		if(isOptoChanged(motor)){
			motor->optoCond.enableOptoJudge = FALSE;
			motor->optoCond.posOpto = motor->step;
			motor->step = 0;    
			motor->pos = 0;     
			SpeedPlanBrake(plan);
		}
		if(SpeedPlanUpdate(plan)){
			UINT32 freq = SpeedPlanFreqGet(plan);
			PulseSetFreq(pulse, freq);
		}
		if(isSpeedPlanDone(plan)){
			Message msg;
			msg.id = motor->msgReady;
			msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
			motor->breakstop = 0;
			
//			StepMotorDisable (motor);  
			PulseStop(pulse);
			MessagePost(&msg);
			motor->pos += (motor->orient)*(motor->step);
			motor->state = MOTOR_STATE_READY;
		}
	}
}

/********************************************************************
 *
 *******************************************************************/
VOID   StepMotorInit    (StepMotor* motor, UINT32 msgReady)
{
	if(motor && motor->lpd  && motor->ori
		     && motor->opto && motor->pulse){
		GpioSet (motor->lpd, MOTOR_LPD_OFF);
		PulseSetOwner (motor->pulse, (VOID*)motor);

		PulseInit (motor->pulse, MotorPulseCallback);
		motor->pos   = 0;
		motor->state = MOTOR_STATE_READY;
		motor->msgReady = msgReady;
		motor->callback = NULL;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID   StepMotorEnable  (StepMotor* motor)
{
	if (motor && motor->lpd){
		GpioSet (motor->lpd, MOTOR_LPD_ON);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID   StepMotorDisable (StepMotor* motor)
{
	if (motor && motor->lpd){
		GpioSet (motor->lpd, MOTOR_LPD_OFF);
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT16 StepMotorGetState(StepMotor* motor)
{
	if(motor) return motor->state;
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
INT32  StepMotorGetPos  (StepMotor* motor)
{
	if(motor) return motor->pos;
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
UINT16  StepMotorGetOpto (StepMotor* motor)
{
	if(motor && motor->opto){
		return GpioGet (motor->opto);
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
VOID   StepMotorSetCallback(StepMotor* motor, StepCallback callback)
{
	if(motor){
		motor->callback = callback;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorMovRel (StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor && motor->ori && motor->pulse) {
		if(step==0){
			motor->step = 0;
			Message msg;
			msg.id = motor->msgReady;
			msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
			MessagePost(&msg);
			return;
		}

		StepMotorEnable  (motor);  //add
		// motor->targetPos = motor->pos + step;
		motor->step = 0;
		motor->state = MOTOR_STATE_RUNNING;

		StepMotorSpeedPlan* plan = &(motor->speedPlan);
		SpeedPlan(plan, step, speed);
		UINT32 freq = SpeedPlanFreqGet(plan);
		PulseSetFreq(motor->pulse, freq);
		if (step < 0){
			motor->orient = -1;
			GpioSet(motor->ori, MOTOR_ORI_BWARD);
		}
		else{
			motor->orient = 1;
			GpioSet(motor->ori, MOTOR_ORI_FWARD);
		}
		PulseStart (motor->pulse);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorMovAbs (StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor) {
		INT32 pos = StepMotorGetPos(motor);
		StepMotorMovRel(motor, (step-pos), speed);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorMovZero(StepMotor* motor, INT32 step, UINT32 speed)
{
	StepMotorMovAbs(motor, 0, speed);
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorMovUntilOpto(StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor && motor->opto){
		motor->optoCond.enableOptoJudge  = TRUE;
		motor->optoCond.stateChangeCount = 0;
		motor->optoCond.stateInit        = GpioGet(motor->opto);
		motor->optoCond.meetOptTimes     = 0;
		motor->optoCond.setChangeOptTimes= 1;
		StepMotorMovRel(motor, step, speed);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorMovUntilOptoTwice(StepMotor* motor, INT32 step, UINT32 speed)
{
	if(motor && motor->opto){
		motor->optoCond.enableOptoJudge  = TRUE;
		motor->optoCond.stateChangeCount = 0;
		motor->optoCond.stateInit        = GpioGet(motor->opto);
		motor->optoCond.meetOptTimes     = 0;
		motor->optoCond.setChangeOptTimes= 2;
		StepMotorMovRel(motor, step, speed);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID StepMotorBrake(StepMotor* motor)
{
//	motor->breakstop = 1;
	StepMotorSpeedPlan* plan = &(motor->speedPlan);
	SpeedPlanBrake(plan);
}

//add
INT32 GetInitOptDistance(StepMotor* motor)
{
	INT32 pos = motor->optoCond.posOpto;
	return pos;
}
INT32 GetMotorMovPos(StepMotor* motor)
{
	INT32 step = (motor->orient)*(motor->step);
	return step;
}


/********************************************************************
 * End of File
 *******************************************************************/


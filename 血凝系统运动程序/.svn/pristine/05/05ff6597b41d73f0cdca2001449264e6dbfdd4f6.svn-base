#include "stateCup.h"

/********************************************************************
 *×¥±­
 *******************************************************************/
VOID CatchCup(Message* msg)
{
	INT32 step;
	switch(MainState.stateManageMid)
	{
		case STATE1_CATCHCUP:{
							TimerStart(200, 1);	
							step = ConfigGet(CFG_GRAB_CATCH_Y_DISTANCE);
							StepMotorMovRel (&(MainState.motors[MOTOR_Y]),step, CATCHING_CUP_SPEED);
							MainState.stateManageMid = STATE1_CUP_MAX;											
					}break;
		case STATE1_CUP_MAX:{
						if((msg->id == MSG_Y_DONE) || (msg->id == MSG_TIMER)){
							MainState.Parameter.savecount1++;
							if(MainState.Parameter.savecount1 >= 2){
								MainState.Parameter.savecount1 = 0;
								GrabStateEndHandler();
							}
						}						
					}break;
		default:break;
	}
	
}
/********************************************************************
 *·Å±­
 *******************************************************************/
VOID PutCup(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_PUTCUP:{
							TimerStart(200, 1);
							INT32 step = ConfigGet(CFG_GRAB_CATCH_Y_DISTANCE) * (-1);
							StepMotorMovRel (&(MainState.motors[MOTOR_Y]), step, PUT_CUP_SPEED);
							MainState.stateManageMid = STATE1_CUP_MAX;								
					}break;
		case STATE1_CUP_MAX:{
						if((msg->id == MSG_Y_DONE) || (msg->id == MSG_TIMER)){
							MainState.Parameter.savecount2++;
							if(MainState.Parameter.savecount2 >= 2){
								MainState.Parameter.savecount2 = 0;
								GrabStateEndHandler();
							}
						}	
					}break;
		default:break;
	}
}




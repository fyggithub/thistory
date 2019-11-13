#include "stateThrowCup.h"



VOID ThrowCupMovement(Message* msg)
{
	INT32 step = 0;
	switch(MainState.stateManageMid)
	{
		case THROW1_Y1:{
						MainState.Parameter.wrkCode = msg->p1;
						MainState.Parameter.throwCupCode = msg->p2;
						if(MainState.Parameter.throwCupCode == 0x6F)
							step = ConfigGet(CFG_GRAB_THROW_Y_POS1);
						else if(MainState.Parameter.throwCupCode == 0x70)
							step = ConfigGet(CFG_GRAB_THROW_Y_POS2);
						StepMotorMovAbs(&(MainState.motors[MOTOR_Y]), step, THROW_CUP_SPEED2);
						MainState.stateManageMid = THROW1_X;
					}break;		
		case THROW1_X:{
						if(msg->id == MSG_Y_DONE){														
							if(MainState.Parameter.throwCupCode == 0x6F){
								step = ConfigGet(CFG_GRAB_THROW_X_POS1);
								MainState.stateManageMid = THROW1_Y2;
							}								
							else if(MainState.Parameter.throwCupCode == 0x70){
								step = ConfigGet(CFG_GRAB_THROW_X_POS2);
								MainState.stateManageMid = THROW1_Z1;
							}
							StepMotorMovAbs(&(MainState.motors[MOTOR_X]), step, THROW_CUP_SPEED1);	
						}							
					}break;
		case THROW1_Y2:{	
						if((msg->id == MSG_X_DONE) || (msg->id == MSG_Z_DONE)){
							step = ConfigGet(CFG_GRAB_THROW_Y_DISTANCE) * (-1);
							StepMotorMovRel (&(MainState.motors[MOTOR_Y]), step, PUT_CUP_SPEED);
							if(MainState.Parameter.throwCupCode == 0x6F)
								MainState.stateManageMid = THROW1_MAX;
							else if(MainState.Parameter.throwCupCode == 0x70)
								MainState.stateManageMid = THROW1_Z2;	
						}
					}break;
		case THROW1_Z1:{
						if(msg->id == MSG_X_DONE){
							step = -1192;
							StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), step, THROW_CUP_SPEED2);
							MainState.stateManageMid = THROW1_Y2;
						}
					}break;
		case THROW1_Z2:{					
						StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), ConfigGet(CFG_GRAB_POS_VRST), THROW_CUP_SPEED2);
						MainState.stateManageMid = THROW1_MAX;
					}break;
		case THROW1_MAX:{
						if((msg->id == MSG_Y_DONE) || (msg->id == MSG_Z_DONE)){
							GrabStateEndHandler();
						}
					}break;
		default:break;
	}
}








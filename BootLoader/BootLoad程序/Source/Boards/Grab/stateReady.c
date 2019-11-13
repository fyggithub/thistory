#include "stateReady.h"

/********************************************************************
 *所有电机方向统一  1为远离光耦、0为靠近光耦     注射电机则 1-吸  0-排
 *******************************************************************/
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir)
{
	switch(motor)
	{
		case MOTOR_X:
					if(dir == 0)   //1为远离光耦，作为正方向
						step = (-1) * step;
					break;
		case MOTOR_Y:
					if(dir)   //Y轴较特殊，正方向（远离光耦）为正脉冲，负方向为负脉冲，其他轴则相反
						step = (-1) * step;
					break;
		case MOTOR_Z:
					if(dir == 0)   //1为远离光耦，作为正方向
						step = (-1) * step;
					break;
		default:break;
	}
	return step;
}

/********************************************************************
 *中位机指令执行完后，一些状态处理及数据发送
 *******************************************************************/
VOID GrabStateEndHandler(VOID)
{
	SendResultMessage();
}

/********************************************************************
 *Basic状态,基本指令运行完后，都会进入此函数
 *******************************************************************/
VOID GrabStateBasicCmdMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:{
				GrabStateEndHandler();   //切换状态，发送结果码
			}break;
		default:break;
	}
}
/********************************************************************
 *Ready状态
 *******************************************************************/
VOID GrabStateReadyMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_HOST_MOTOR_ENABLE:{		
							UINT8 motor = msg->p1 - 1;
							UINT8 state = msg->p2;
							if(state)
								StepMotorEnable(&(MainState.motors[motor]));
							else if(state == 2)
								StepMotorDisable(&(MainState.motors[motor]));
							GrabStateEndHandler();
						}break;					
		case MSG_HOST_MOTOR_RESET:{
							MainState.stateManageTop = STATE0_RESET;
							MainState.stateManageMid = STATE1_OPT_INIT;
							GrabStateMotorResetMessageHandler(msg);
						}break;	
		case MSG_HOST_MOTOR_TOZERO:{
							UINT8 motor = msg->p1 - 1;
							UINT16 speed = msg->p2;
							switch(motor){
								case 0:StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
								case 1:StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
								case 2:StepZAxisMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
								default:break;							
							}							
							MainState.stateManageTop = STATE0_BASIC;
						}break;							
		case MSG_HOST_MOTOR_MOVREL:{
							UINT8 motor = msg->p1 - 1;
							UINT16 speed = msg->p2;
							INT32 step = msg->p3;
							INT16 dir = msg->p4;
							step = DirSortOut(motor,step,dir);	
							switch(motor){
								case 0:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
								case 1:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
								case 2:StepZAxisMotorMovRel (&(MainState.motors[motor]), step, speed); break;
								default:break;
							}							
							MainState.stateManageTop = STATE0_BASIC;
						}break;								
		case MSG_HOST_MOTOR_MOVABS:{
							UINT8 motor = msg->p1 - 1;
							UINT16 speed = msg->p2;
							INT32 step = msg->p3;
							switch(motor){
								case 0:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
								case 1:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
								case 2:StepZAxisMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
								default:break;
							}							
							MainState.stateManageTop = STATE0_BASIC;
						}break;								
		case MSG_HOST_MOTOR_MOVWRK:{
							UINT8 motor = msg->p1;
							UINT16 wrkpos = msg->p2;
							UINT16 speed = msg->p3;			
							GrabWrkStateMessageHandler(motor, wrkpos, speed,0);
							MainState.stateManageTop = STATE0_BASIC;
						}break;				
		case MSG_HOST_MOTOR_INQUIRE:break;
		case MSG_HOST_MOTOR_MIXCTL:{
							MainState.stateManageTop = STATE0_MIX_CTRL;
							MainState.stateManageMid = MIX1_START;
							MixMotorCtrl(msg);
						}break;
		case MSG_HOST_MOTOR_MOVXY:{
							MainState.stateManageTop = STATE0_MV;
							MainState.stateManageMid = STATE1_MV_BASIC;
							GrabStateMvMessageHandler(msg);
						}break;							
		case MSG_HOST_MOTOR_ZAXISPOS:{
							MainState.stateManageTop = STATE0_ZAXISPOS;
							MainState.stateManageMid = STATE1_ZAXIS_MV;
							ZAxisSpecifyVerticalPos(msg);
						}break;
		case MSG_HOST_MOTOR_CATCHCUP:{
							MainState.stateManageTop = STATE0_CATCH;
							MainState.stateManageMid = STATE1_CATCHCUP;
							CatchCup(msg);
						}break;
		case MSG_HOST_MOTOR_PUTCUP:{
							MainState.stateManageTop = STATE0_PUT;
							MainState.stateManageMid = STATE1_PUTCUP;
							PutCup(msg);
						}break;
		case MSG_HOST_MOTOR_MIXTIME:{
							MainState.stateManageTop = STATE0_MIX_TIME;
							MainState.stateManageMid = ZAXIS1_RISE;
							ZAxisMixMovement(msg);
						}break;
		case MSG_HOST_MOTOR_MVMIX:{
							MainState.stateManageTop = STATE0_MV_MIX;
							MainState.stateManageMid = ZAXIS1_BASIC;
							XYMovementMix(msg);
						}break;
		case MSG_HOST_MOTOR_THROWCUP:{
							MainState.stateManageTop = STATE0_THROW;
							MainState.stateManageMid = THROW1_Y1;
							ThrowCupMovement(msg);
						}break;
		default:break;	//something is wrong								
	}
}







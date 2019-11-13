#include "stateReset.h"

/********************************************************************
 *
 *******************************************************************/
STATIC VOID ExitOptState(Message* msg)
{
	switch(msg->id)   		//最终清理状态
	{
		case MSG_X_DONE:	MainState.Parameter.resetCount[0] = 1 ;	break;
		case MSG_Y_DONE:	MainState.Parameter.resetCount[1] = 1 ;	break;
		case MSG_Z_DONE:	MainState.Parameter.resetCount[2] = 1 ;	break;									
		default:break;
	}
	GrabStateEndHandler();	//发送结果码,注意此函数里将状态切换到Ready状态了
	
	if( !(MainState.Parameter.resetCount[0] && MainState.Parameter.resetCount[1] && MainState.Parameter.resetCount[2]) )
		MainState.stateManageTop = STATE0_INIT;
}
/********************************************************************
 *
 *******************************************************************/
STATIC INT32 PlusePositiveDir(Message* msg,INT32 mvpluse,INT32 dir)
{
	switch(msg->id)   		
	{
		case MSG_X_DONE:{
				if(dir == MOTOR_X_POSITIVE_DIRECTION)
					mvpluse = (-1)*mvpluse;
			}break;
		case MSG_Y_DONE:{	
				if(dir == MOTOR_Y_POSITIVE_DIRECTION)
					mvpluse = (-1)*mvpluse;	
			}break;
		case MSG_Z_DONE:{	
				if(dir == MOTOR_Z_POSITIVE_DIRECTION)
					mvpluse = (-1)*mvpluse;	
			}break;				
		default:break;
	}
	return (mvpluse);
}
/********************************************************************
 *Enter and leave and reenter
 *******************************************************************/
STATIC VOID GrabResetNoOptProcessHandle2(Message* msg, StepMotor* motor, INT32 step, UINT32 speed)
{
	INT32 outoptpluse = 0;
	switch(MainState.stateManageBtm)
	{
		case STATE2_OPT_NOOPT_INIT:{
							switch(msg->id)   		//离开光耦
							{
								case MSG_X_DONE:
								case MSG_Y_DONE:
								case MSG_Z_DONE:							
											if(GetInitOptDistance(motor) >= 3200)   //if moro then 3200,direct stop
											{
												ExitOptState(msg);
												MainState.stateManageMid = STATE1_OPT_MAX;																																		
												MainState.stateManageBtm = STATE2_OPT_NOOPT_MAX;
											}else{
												if(msg->id == MSG_Z_DONE){
													outoptpluse = PlusePositiveDir(msg,320,1);
													StepZAxisMotorMovRel (motor, outoptpluse, speed);
												}
												else{
													outoptpluse = PlusePositiveDir(msg,3200,1);																								
													StepMotorMovRel (motor, outoptpluse, speed);
												}
												MainState.stateManageBtm = STATE2_OPT_NOOPT_LEAVE;
											}													
											break;
								default:break;
							}
						}break;
		case STATE2_OPT_NOOPT_LEAVE:{
						switch(msg->id)   		//再次进入光耦
						{
							case MSG_X_DONE:{
										StepMotorMovUntilOpto(motor,step,speed);																																			
										MainState.stateManageBtm = STATE2_OPT_NOOPT_AGENT;
									}break;
							case MSG_Y_DONE:{
										StepMotorMovUntilOpto(motor,step,speed);																																			
										MainState.stateManageBtm = STATE2_OPT_NOOPT_AGENT;
									}break;
							case MSG_Z_DONE:{
										StepZAxisMotorMovUntilOpto(motor,step,speed);																																												
										MainState.stateManageBtm = STATE2_OPT_NOOPT_AGENT;
									}break;
							default:break;
						}
					}break;
		case STATE2_OPT_NOOPT_AGENT:{
						switch(msg->id)   		//防止其他消息进入，再次进入光耦
						{
							case MSG_X_DONE:
							case MSG_Y_DONE:
							case MSG_Z_DONE:{																			
											ExitOptState(msg);								
											MainState.stateManageMid = STATE1_OPT_MAX;																																		
											MainState.stateManageBtm = STATE2_OPT_NOOPT_MAX;
										}break;
							default:break;
						}																					
					}break;
		case STATE2_OPT_NOOPT_MAX:break;
		default:break;
	}
}
/********************************************************************
 *Leave after entering
 *******************************************************************/
STATIC VOID GrabResetOptProcessHandle(Message* msg,StepMotor* motor, INT32 step, UINT32 speed)
{	
	INT32 outoptpluse = 0;
	switch(MainState.stateManageBtm)
	{
		case STATE2_OPT_OPT_INIT:  break;       				//not add break
		case STATE2_OPT_OPT_LEAVE:{
								if(GpioGet(MainState.motors[MainState.Parameter.motor].opto)){   //离开光耦
									switch(msg->id)   		//防止其他消息进入，再次进入光耦
									{
										case MSG_X_DONE:{
														StepMotorMovUntilOpto(motor, step,speed);																	
														MainState.stateManageBtm = STATE2_OPT_OPT_AGENT;
													}break;
										case MSG_Y_DONE:{
														StepMotorMovUntilOpto(motor, step,speed);																	
														MainState.stateManageBtm = STATE2_OPT_OPT_AGENT;
													}break;
										case MSG_Z_DONE:{
														StepZAxisMotorMovUntilOpto(motor, step,speed);																											
														MainState.stateManageBtm = STATE2_OPT_OPT_AGENT;
													}break;
										default:break;
									}
								}
								else{  //如果移动后没有离开光耦，接着往后继续移动一段距离
									if(msg->id == MSG_Z_DONE){
										outoptpluse = DirSortOut(MainState.Parameter.motor,320,1);
										StepZAxisMotorMovRel (&(MainState.motors[MainState.Parameter.motor]), outoptpluse, speed);
									}
									else{
										outoptpluse = DirSortOut(MainState.Parameter.motor,3200,1);
										StepMotorMovRel (&(MainState.motors[MainState.Parameter.motor]), outoptpluse, speed);
									}
								}									
							}break;
		case STATE2_OPT_OPT_AGENT:{
								switch(msg->id)   		//防止其他消息进入，再次进入光耦
								{
									case MSG_X_DONE:
									case MSG_Y_DONE:
									case MSG_Z_DONE:{																			
													ExitOptState(msg);									
													MainState.stateManageMid = STATE1_OPT_MAX;
													MainState.stateManageBtm = STATE2_OPT_OPT_MAX;
												}break;
									default:break;
								}																
							}break;												
		case STATE2_OPT_OPT_MAX:	 break;
		default:break;
	}
}
/********************************************************************
 *电机复位
 *******************************************************************/
VOID GrabStateMotorResetMessageHandler(Message* msg)
{
	INT32 outoptpluse = 0;
	INT32 max_distance[5] = {-36000,16000,-3200};
	
	switch(MainState.stateManageMid)
	{
		case STATE1_OPT_INIT:{
							MainState.Parameter.motor = msg->p1 - 1;				
							MainState.Parameter.speed = msg->p2; 							
							if(GpioGet(MainState.motors[MainState.Parameter.motor].opto)){   //光耦未遮挡状态							
								switch(MainState.Parameter.motor){
									case MOTOR_X:{
												StepMotorMovUntilOpto(&(MainState.motors[MainState.Parameter.motor]), \
														max_distance[MainState.Parameter.motor], MainState.Parameter.speed);
											}break;
									case MOTOR_Y:{
												StepMotorMovUntilOpto(&(MainState.motors[MainState.Parameter.motor]), \
														max_distance[MainState.Parameter.motor], MainState.Parameter.speed);
											}break;
									case MOTOR_Z:{
												StepZAxisMotorMovUntilOpto(&(MainState.motors[MainState.Parameter.motor]), \
														max_distance[MainState.Parameter.motor], MainState.Parameter.speed);
											}break;
									default:break;
								}																
								MainState.stateManageMid = STATE1_NOOPT;
								MainState.stateManageBtm = STATE2_OPT_NOOPT_INIT;							
							}							
							else{    								   //光耦遮挡状态																		
								switch(MainState.Parameter.motor)
								{
									case MOTOR_X:{
												outoptpluse = DirSortOut(MainState.Parameter.motor,3200,1);
												StepMotorMovRel (&(MainState.motors[MainState.Parameter.motor]), outoptpluse, MainState.Parameter.speed);
											}break;
									case MOTOR_Y:{
												outoptpluse = DirSortOut(MainState.Parameter.motor,3200,1);
												StepMotorMovRel (&(MainState.motors[MainState.Parameter.motor]), outoptpluse, MainState.Parameter.speed);
											}break;
									case MOTOR_Z:{
												outoptpluse = DirSortOut(MainState.Parameter.motor,320,1); 
												StepZAxisMotorMovRel (&(MainState.motors[MainState.Parameter.motor]), outoptpluse, MainState.Parameter.speed);
											}break;								
									default:break;
								}								
								MainState.stateManageMid = STATE1_OPT;
								MainState.stateManageBtm = STATE2_OPT_OPT_LEAVE;
							}
						}break;
		case STATE1_OPT: 	GrabResetOptProcessHandle(msg, &(MainState.motors[MainState.Parameter.motor]),\
														max_distance[MainState.Parameter.motor], MainState.Parameter.speed);		
							break;					
		case STATE1_NOOPT:	GrabResetNoOptProcessHandle2(msg, &(MainState.motors[MainState.Parameter.motor]),\
															max_distance[MainState.Parameter.motor],  MainState.Parameter.speed);	
							break;																				
		case STATE1_OPT_MAX:	break;
		default:break;
	}	
}








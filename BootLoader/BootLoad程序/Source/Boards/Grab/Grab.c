#include "Grab.h"

mainState MainState;
/********************************************************************
 *
 *******************************************************************/
STATIC VOID GrabStateMessageHandler(Message* msg)
{
	switch(MainState.stateManageTop)
	{
		case STATE0_INIT:{	
						if(msg->id == MSG_HOST_MOTOR_RESET){
							MainState.stateManageTop = STATE0_RESET;
							MainState.stateManageMid = STATE1_OPT_INIT;
							GrabStateMotorResetMessageHandler(msg);
						}
					}break;
		case STATE0_RESET:	 GrabStateMotorResetMessageHandler(msg);	break;				
		case STATE0_READY:	 GrabStateReadyMessageHandler(msg);			break;
		case STATE0_BASIC:	 GrabStateBasicCmdMessageHandler(msg);		break;
		case STATE0_MIX_CTRL:MixMotorCtrl(msg);							break;
		case STATE0_MV:		 GrabStateMvMessageHandler(msg);			break;				
		case STATE0_ZAXISPOS:ZAxisSpecifyVerticalPos(msg);		 		break;
		case STATE0_CATCH:	 CatchCup(msg);								break;
		case STATE0_PUT:	 PutCup(msg);								break;
		case STATE0_MIX_TIME:ZAxisMixMovement(msg);						break;
		case STATE0_MV_MIX:	 XYMovementMix(msg);						break;
		case STATE0_THROW:	 ThrowCupMovement(msg);						break;
		case STATE0_DELAY:{
							if(msg->id == MSG_TIMER){
								GrabStateEndHandler();
							}
						}break;
		default:break;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID GrabMessageHandler(Message* msg)
{
	switch(msg->id)
	{	
		case MSG_HOST_VERSION:	SendResultMessage();		break;
		case MSG_HOST_GETINFO:			break;
		case MSG_HOST_SETINFO:			break;
		case MSG_HOST_PARINFO:{
									MainState.Parameter.codeID1 = msg->p1;
									MainState.Parameter.codeID2 = msg->p2;
									MainState.Parameter.codeID3 = msg->p3;
									MainState.Parameter.codeID4 = msg->p4;
									SendResultMessage();
							}break;
		case MSG_HOST_PARSET:{											
									INT32 id1 = msg->p1;
									INT32 id2 = msg->p2;
									INT32 value1 = msg->p3;
									INT32 value2 = msg->p4;
									if((id1 == 0xFFFF) || (id2 == 0xFFFF)){
										ConfigSetDone();     //CRC校验保存F-RAM
									}
									else{
										if(id1 >= CFG_GRAB_POSLMT_VNEG)
											ConfigSetDword(id1, value1);
										if(id2 >= CFG_GRAB_POSLMT_VNEG)
											ConfigSetDword(id2, value2);
									}
									SendResultMessage();
							}break;
		case MSG_HOST_WRKSAVE:{
									INT16 work = msg->p1;
									INT32 step1 = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
									INT32 step2 = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
									switch(work)
									{
										case GRAB_MEASUREMENTBIT_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MESURE_X_POS1, step1);
																ConfigSetDword(CFG_GRAB_MESURE_Y_POS1, step2);												
															}break;
										case (GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM - 1):{											
																ConfigSetDword(CFG_GRAB_MESURE_X_POS12, step1);
																ConfigSetDword(CFG_GRAB_MESURE_Y_POS12, step2);												
															}break;
										case GRAB_INCUBATIONBIT_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS21, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS21, step2);												
															}break;
										case (GRAB_INCUBATIONBIT_STARTCODE+5):{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS26, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS26, step2);												
															}break;	
										case (GRAB_INCUBATIONBIT_STARTCODE+17):{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS38, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS38, step2);												
															}break;
										case GRAB_MAGNETICBEADCUP_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MEGNETIC_X_POS61, step1);
																ConfigSetDword(CFG_GRAB_MEGNETIC_Y_POS61, step2);												
															}break;
										case (GRAB_MAGNETICBEADCUP_STARTCODE+11):{											
																ConfigSetDword(CFG_GRAB_MEGNETIC_X_POS72, step1);
																ConfigSetDword(CFG_GRAB_MEGNETIC_Y_POS72, step2);												
															}break;
										case GRAB_MAGNETICBEAD_MEASURE_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MAGMEA_X_POS, step1);
																ConfigSetDword(CFG_GRAB_MEGMEA_Y_POS, step2);												
															}break;
										case GRAB_COOPERATION_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_DISK_X_POS, step1);
																ConfigSetDword(CFG_GRAB_DISK_Y_POS, step2);												
															}break;
										case (GRAB_COOPERATION_STARTCODE+1):{											
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_XPOS, step1);
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_YPOS, step2);												
															}break;
										case (GRAB_COOPERATION_STARTCODE+2):{											
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_XPOS, step1);
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_YPOS, step2);												
															}break;
										case GRAB_THROWCUP_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_THROW_X_POS1, step1);
																ConfigSetDword(CFG_GRAB_THROW_Y_POS1, step2);												
															}break;					
										case (GRAB_THROWCUP_STARTCODE+1):{											
																ConfigSetDword(CFG_GRAB_THROW_X_POS2, step1);
																ConfigSetDword(CFG_GRAB_THROW_Y_POS2, step2);												
															}break;
										case 257:	{	//Z轴初始高度
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_POS_VRST, step3);																								
													}break;
										case 258:	{	//Z轴测量抓杯高度1
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS, step3);	//光学测量垂直位置																								
													}break;
										case 2818:	{	//Z轴测量抓杯高度12
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS12, step3);	//光学测量垂直位置																								
													}break;
										case 5378:	{	//Z轴孵育抓杯高度21
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS, step3);	//光学孵育垂直位置																							
													}break;
										case 6658:	{	//Z轴孵育抓杯高度26
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS26, step3);	//光学孵育垂直位置																							
													}break;
										case 9730:	{	//Z轴孵育抓杯高度38
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS38, step3);	//光学孵育垂直位置																							
													}break;
										case 15618:	{	//磁珠杯垂直位置61
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS, step3);	//磁珠杯垂直位置																						
													}break;
										case 18434:	{	//磁珠杯垂直位置72
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS72, step3);	//磁珠杯垂直位置																						
													}break;
										case 13058:	{	//磁珠法测量位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGMEA_CATCH_POS, step3);	//磁珠法测量位置																						
													}break;
										case 25858:	{	//调度盘
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_DISK_CATCH_POS, step3);	//调度盘																					
													}break;
										case 259:	{	//Z轴放杯高度
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS, step3);	//光学测量垂直位置																					
													}break;
										case 5379:	{	//Z轴孵育抓杯高度，放杯
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS, step3);	//光学孵育垂直位置																							
													}break;
										case 10499:	{	//磁珠杯垂直位置，放杯
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS, step3);	//磁珠杯垂直位置																						
													}break;
										case 13059:	{	//磁珠法测量位置，放杯
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGMEA_CATCH_POS, step3);	//磁珠法测量位置																						
													}break;
										case 25859:	{	//调度盘，放杯
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_DISK_CATCH_POS, step3);	//调度盘																					
													}break;
										case 26116:	{	//Z轴吸排液高度，样本
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_VERTICAL, step3);																								
													}break;
										case 26372:	{	//Z轴吸排液高度，试剂
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_VERTICAL, step3);																								
													}break;
										case 261:	{	//Z轴混匀高度
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MIX_HIGHT, step3);																								
													}break;
										case 28422:	{	//Z轴抛杯位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_THROW_VERTICAL, step3);																								
													}break;																																																
										default:break;  	 //发送的工作位置错误
									}							
									ConfigSetDone();
									SendResultMessage();
							}break;
		case MSG_HOST_MOTOR_INQUIRE:   SendResultMessage();	 break;
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_MIX_DONE:
		case MSG_HOST_MOTOR_ENABLE:
		case MSG_HOST_MOTOR_RESET:		
		case MSG_HOST_MOTOR_TOZERO:
		case MSG_HOST_MOTOR_MOVREL:
		case MSG_HOST_MOTOR_MOVABS:
		case MSG_HOST_MOTOR_MOVWRK:
		case MSG_HOST_MOTOR_MIXCTL:
		case MSG_HOST_MOTOR_MIXINQUIRE:
		case MSG_HOST_MOTOR_MOVXY:
		case MSG_HOST_MOTOR_ZAXISPOS:
		case MSG_HOST_MOTOR_CATCHCUP:
		case MSG_HOST_MOTOR_PUTCUP:
		case MSG_HOST_MOTOR_MIXTIME:
		case MSG_HOST_MOTOR_MVMIX:
		case MSG_HOST_MOTOR_THROWCUP:	GrabStateMessageHandler(msg);	break;
		case MSG_HOST_MOTOR_PUMPSET:
		case MSG_HOST_MOTOR_PUMPGET:
		case MSG_HOST_MOTOR_VALVESET:
		case MSG_HOST_MOTOR_VALVEGET:
		case MSG_HOST_MOTOR_IOSET:
		case MSG_HOST_MOTOR_IOGET:
		case MSG_HOST_MOTOR_OPTOGET:
		case MSG_HOST_MOTOR_FAULT:		break;
		case MSG_DELAY:{
						MainState.stateManageTop = STATE0_DELAY;	
						INT16 count = msg->p1 / 100;
						TimerStart(100, count);
					}break;
		case MSG_STOP:{
							if(StepMotorGetState(&(MainState.motors[MOTOR_X])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_X]));
							if(StepMotorGetState(&(MainState.motors[MOTOR_Y])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_Y]));
							if(StepMotorGetState(&(MainState.motors[MOTOR_Z])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_Z]));						
																						//停止电机
							memset(&MainState.Parameter,0,sizeof(MainState.Parameter)); //清空数据															
							PwmStop();													//关闭混匀电机
							TimerStop(); 												//关闭定时器
							TimerStop2();
							MessageQueueInit();											//清空消息队列
							
							SendResultMessage();										//发送结果码
							MainState.stateManageTop = STATE0_INIT;    //接下来只能复位,注意此时除复位命令，其它指令全部返回错误
						}break;
		default:	break;//something is wrong		
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID GrabInit(VOID)
{
	UINT8 tag = DebugRegister ("grab");
	MessageRegisterHandler (GrabMessageHandler);
	DebugLog(tag, "Grab App Init.");
	
	MainState.motors[MOTOR_X].lpd 	 = BoardGetGpio (IO_LPD1);
	MainState.motors[MOTOR_Y].lpd 	 = BoardGetGpio (IO_LPD2);
	MainState.motors[MOTOR_Z].lpd 	 = BoardGetGpio (IO_LPD3);
	MainState.motors[MOTOR_X].ori 	 = BoardGetGpio (IO_ORI1);
	MainState.motors[MOTOR_Y].ori 	 = BoardGetGpio (IO_ORI2);
	MainState.motors[MOTOR_Z].ori 	 = BoardGetGpio (IO_ORI3);
	MainState.motors[MOTOR_X].opto 	 = BoardGetGpio (IO_OPTO1);
	MainState.motors[MOTOR_Y].opto 	 = BoardGetGpio (IO_OPTO2);
	MainState.motors[MOTOR_Z].opto 	 = BoardGetGpio (IO_OPTO3);
	MainState.motors[MOTOR_X].pulse   =	BoardGetPulse(MOTOR_X);
	MainState.motors[MOTOR_Y].pulse   =	BoardGetPulse(MOTOR_Y);
	MainState.motors[MOTOR_Z].pulse   =	BoardGetPulse(MOTOR_Z);
	MainState.motors[MOTOR_X].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_X);   //到时候需要改电机台阶
	MainState.motors[MOTOR_Y].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Y);
	MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Z_DOWN);
	
	StepMotorInit(&(MainState.motors[MOTOR_X]),MSG_X_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Y]),MSG_Y_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Z]),MSG_Z_DONE );
	
	MainState.stateManageTop = STATE0_READY;
}



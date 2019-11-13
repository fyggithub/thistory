#include "Sample.h"

mainState MainState;
/********************************************************************
 *
 *******************************************************************/
STATIC VOID SampleStateMessageHandler(Message* msg)
{
	switch(MainState.stateManageTop)
	{
		case STATE0_INIT:{	
						if(msg->id == MSG_HOST_MOTOR_RESET)
						{
							MainState.stateManageTop = STATE0_RESET;
							MainState.stateManageMid = STATE1_OPT_INIT;
							SampleStateMotorResetMessageHandler(msg);
						}
					}break;
		case STATE0_RESET:	SampleStateMotorResetMessageHandler(msg);	break;				
		case STATE0_READY:	SampleStateReadyMessageHandler(msg);		break;
		case STATE0_BASIC:	SampleStateBasicCmdMessageHandler(msg);		break;			
		case STATE0_ZAIXDET:SampleStateMotorZaixDetMessageHandler(msg);	break;
		case STATE0_MV:		SampleStateMvMessageHandler(msg);			break;
		case STATE0_SUBBUF:	SampleStateSubMessageHandler(msg);			break;
		case STATE0_SUB:	SampleStateSubMessageHandler(msg);			break;
		case STATE0_DRA:	SampleStateDraMessageHandler(msg);			break;
		case STATE0_DRABUF:	SampleStateDraMessageHandler(msg);			break;
		case STATE0_CLN:	SampleStateClnMessageHandler(msg); 			break;
		case STATE0_DELAY:{
							if(msg->id == MSG_TIMER){
								SampleStateEndHandler();
							}
						}break;
		default:break;
	}
}

VOID SampleMessageHandler(Message* msg)
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
										if(id1 >= CFG_SAMPLE_POSLMT_VNEG)
											ConfigSetDword(id1, value1);
										if(id2 >= CFG_SAMPLE_POSLMT_VNEG)
											ConfigSetDword(id2, value2);
									}
									SendResultMessage();
							}break;
		case MSG_HOST_WRKSAVE:{
									INT32 work = msg->p1;
									INT32 step1 = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
									INT32 step2 = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
									switch(work)
									{
										case (SAMPLE_RACK_STARTCODE):{										//样本管位								
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_X_POS1, step1);
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_Y_POS1, step2);												
															}break;
										case (SAMPLE_RACK_STARTCODE + 9):{
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_X_POS2, step1);
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_Y_POS2, step2);
															}break;
										case (SAMPLE_RACK_STARTCODE + 70):{
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_X_POS3, step1);
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_Y_POS3, step2);
															}break;	
										case SAMPLE_COOLBIT_STARTCODE:{										//冷藏试剂位
																ConfigSetDword(CFG_SAMPLE_COOL_X_POS1, step1);
																ConfigSetDword(CFG_SAMPLE_COOL_Y_POS2, step2);
															}break;
										case (SAMPLE_COOLBIT_STARTCODE + SAMPLE_COOLBIT_NUM - 1):{
																ConfigSetDword(CFG_SAMPLE_COOL_X_POS2, step1);
																ConfigSetDword(CFG_SAMPLE_COOL_Y_POS2, step2);
															}break;
										case SAMPLE_ROOMTEMPERATURE_STARTCODE:{                              //常温试剂位
																ConfigSetDword(CFG_SAMPLE_ROOM_X_POS1, step1);
																ConfigSetDword(CFG_SAMPLE_ROOM_Y_POS1, step2);
															}break;
										case (SAMPLE_ROOMTEMPERATURE_STARTCODE + SAMPLE_ROOMTEMPERATURE_NUM - 1):{ 
																ConfigSetDword(CFG_SAMPLE_ROOM_X_POS2, step1);
																ConfigSetDword(CFG_SAMPLE_ROOM_Y_POS2, step2);
															}break;					
										case (SAMPLE_NEEDLESHORIZONTAL_STARTCODE):{   //清洗池位置
																ConfigSetDword(CFG_SAMPLE_CLEAN_X_POS, step1);
																ConfigSetDword(CFG_SAMPLE_CLEAN_Y_POS, step2);
															}break;
										case (SAMPLE_NEEDLESHORIZONTAL_STARTCODE+1):{ //调度盘位置
																ConfigSetDword(CFG_SAMPLE_DISK_X_POS, step1);
																ConfigSetDword(CFG_SAMPLE_DISK_Y_POS, step2);
															}break;
										case (SAMPLE_NEEDLESHORIZONTAL_STARTCODE+2):{ //配合抓杯手
																ConfigSetDword(CFG_SAMPLE_COOPERATE_X_POS, step1);
																ConfigSetDword(CFG_SAMPLE_COOPERATE_Y_POS, step2);
															}break;
										case 259:			{ //样本架垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_SAMPLEAREA_UP_POS, step3);
															}break;
										case 38659:			{ //冷藏试剂垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_COOL_UP_POS, step3);
															}break;
										case 46339:			{ //常温试剂垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_ROOM_UP_POS, step3);
															}break;
										case 51459:			{ //清洗池垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_CLEAN_UP_POS, step3);
															}break;
										case 51715:			{ //调度盘垂直高度
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_DISK_UP_POS, step3);
															}break;
										case 51971:			{ //配合抓手垂直高度
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_SAMPLE_COOPERATE_UP_POS, step3);
															}break;
										default:break;  	 //发送的工作位置错误
									}
									ConfigSetDone();
									SendResultMessage();
							}break;
		case MSG_HOST_MOTOR_INQUIRE:	SendResultMessage();	break;
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_SYN_DONE:
		case MSG_TIR_DONE:
		case MSG_DET_DONE:
		case MSG_HOST_MOTOR_ENABLE:
		case MSG_HOST_MOTOR_RESET:		
		case MSG_HOST_MOTOR_TOZERO:
		case MSG_HOST_MOTOR_MOVREL:
		case MSG_HOST_MOTOR_MOVABS:
		case MSG_HOST_MOTOR_MOVWRK:
		case MSG_HOST_MOTOR_ZAIXDET:
		case MSG_HOST_MOTOR_SUBDRA:                
		case MSG_HOST_MOTOR_MOVXY:
		case MSG_HOST_MOTOR_ZAXISSUBBUF:
		case MSG_HOST_MOTOR_ZAXISSUB:
		case MSG_HOST_MOTOR_ZAXISDRA:
		case MSG_HOST_MOTOR_ZAXISDRABUF:
		case MSG_HOST_MOTOR_ZAXISCLE:		SampleStateMessageHandler(msg);	break;
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
							if(StepMotorGetState(&(MainState.motors[MOTOR_SYN])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_SYN]));
																						//停止电机
							memset(&MainState.Parameter,0,sizeof(MainState.Parameter)); //清空数据
							LiquidneedleDisable();										//关闭外部中断
							TimerStop(); 												//关闭定时器
							TimerStop2();
							MessageQueueInit();											//清空消息队列
							
							SendResultMessage();										//发送结果码
							MainState.stateManageTop = STATE0_INIT;    //接下来只能复位,注意此时除复位命令，其它指令全部返回错误
						}break;
		default:	break;//something is wrong		
	}
}

VOID SampleInit(VOID)
{
	UINT8 tag = DebugRegister ("sample");
	MessageRegisterHandler (SampleMessageHandler);
	DebugLog(tag, "Sample App Init.");
	
	MainState.motors[MOTOR_X].lpd 	 = BoardGetGpio (IO_LPD1);
	MainState.motors[MOTOR_Y].lpd 	 = BoardGetGpio (IO_LPD2);
	MainState.motors[MOTOR_Z].lpd 	 = BoardGetGpio (IO_LPD3);
	MainState.motors[MOTOR_SYN].lpd  = BoardGetGpio (IO_LPD4);
	MainState.motors[MOTOR_X].ori 	 = BoardGetGpio (IO_ORI1);
	MainState.motors[MOTOR_Y].ori 	 = BoardGetGpio (IO_ORI2);
	MainState.motors[MOTOR_Z].ori 	 = BoardGetGpio (IO_ORI3);
	MainState.motors[MOTOR_SYN].ori  = BoardGetGpio (IO_ORI4);
	MainState.motors[MOTOR_X].opto 	 = BoardGetGpio (IO_OPTO1);
	MainState.motors[MOTOR_Y].opto 	 = BoardGetGpio (IO_OPTO2);
	MainState.motors[MOTOR_Z].opto 	 = BoardGetGpio (IO_OPTO3);
	MainState.motors[MOTOR_SYN].opto = BoardGetGpio (IO_OPTO4);	
	MainState.motors[MOTOR_X].pulse   =	BoardGetPulse(MOTOR_X);
	MainState.motors[MOTOR_Y].pulse   =	BoardGetPulse(MOTOR_Y);
	MainState.motors[MOTOR_Z].pulse   =	BoardGetPulse(MOTOR_Z);
	MainState.motors[MOTOR_SYN].pulse =	BoardGetPulse(MOTOR_SYN);
	MainState.motors[MOTOR_X].speedPlan.accTable   = GetAccelerateTable (ACCTBL_SAMPLE_X);
	MainState.motors[MOTOR_Y].speedPlan.accTable   = GetAccelerateTable (ACCTBL_SAMPLE_Y);
	MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_SAMPLE_Z_DOWN);
	MainState.motors[MOTOR_SYN].speedPlan.accTable = GetAccelerateTable (ACCTBL_SAMPLE_SYN_DOWN);
	
	StepMotorInit(&(MainState.motors[MOTOR_X]),MSG_X_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Y]),MSG_Y_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Z]),MSG_Z_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_SYN]),MSG_SYN_DONE );
	
	MainState.stateManageTop = STATE0_READY;
}














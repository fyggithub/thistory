#include "Regent.h"

mainState MainState;

/********************************************************************
 *
 *******************************************************************/
STATIC VOID RegeStateMessageHandler(Message* msg)
{
	switch(MainState.stateManageTop)
	{
		case STATE0_INIT:{	
						if(msg->id == MSG_REGEHOST_MOTOR_RESET)
						{
							MainState.stateManageTop = STATE0_RESET;
							MainState.stateManageMid = STATE1_OPT_INIT;
							RegeStateMotorResetMessageHandler(msg);
						}
					}break;
		case STATE0_RESET:		RegeStateMotorResetMessageHandler(msg);		break;				
		case STATE0_READY:		RegeStateReadyMessageHandler(msg);			break;
		case STATE0_ZAIXDET:	RegeStateMotorZaixDetMessageHandler(msg);	break;
		case STATE0_BASIC:		RegeStateBasicCmdMessageHandler(msg);		break;	
		case STATE0_MV:			RegeStateMvMessageHandler(msg);				break;
		case STATE0_SUB:		RegeStateSubMessageHandler(msg);			break;
		case STATE0_DRA:		RegeStateDraMessageHandler(msg);			break;
		case STATE0_CLN:		RegeStateClnMessageHandler(msg); 			break;
		case STATE0_DELAY:{
							if(msg->id == MSG_TIMER){
								RegeStateEndHandler();
							}
						}break;       
		default:break;
	}
}

/********************************************************************
 *
 *******************************************************************/
VOID RegeMessageHandler(Message* msg)
{
	switch(msg->id)
	{	
		case MSG_REGEHOST_VERSION:	SendResultMessage();		break;
		case MSG_REGEHOST_GETINFO:			break;
		case MSG_REGEHOST_SETINFO:			break;
		case MSG_REGEHOST_PARINFO:{
									MainState.Parameter.codeID1 = msg->p1;
									MainState.Parameter.codeID2 = msg->p2;
									MainState.Parameter.codeID3 = msg->p3;
									MainState.Parameter.codeID4 = msg->p4;
									SendResultMessage();
								}break;
		case MSG_REGEHOST_PARSET:{			
									INT32 id1 = msg->p1;
									INT32 id2 = msg->p2;
									INT32 value1 = msg->p3;
									INT32 value2 = msg->p4;
									if((id1 == 0xFFFF) || (id2 == 0xFFFF)){
										ConfigSetDone();     //CRC校验保存F-RAM
									}
									else{
										if(id1 >= CFG_REAGENT_POSLMT_VNEG)
											ConfigSetDword(id1, value1);
										if(id2 >= CFG_REAGENT_POSLMT_VNEG)
											ConfigSetDword(id2, value2);
									}
									SendResultMessage();
								}break;
		case MSG_REGEHOST_WRKSAVE:{
									INT16 work = msg->p1;
									INT32 step1 = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
									INT32 step2 = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
									switch(work)
									{
										case (REAGENT_MIDHOLE_CODE + 2):{											
																ConfigSetDword(CFG_REAGENT_COOL_X_POS1, step1);
																ConfigSetDword(CFG_REAGENT_COOL_Y_POS1, step2);												
															}break;
										case (REAGENT_MIDHOLE_CODE + 11):{
																ConfigSetDword(CFG_REAGENT_COOL_X_POS2, step1);
																ConfigSetDword(CFG_REAGENT_COOL_Y_POS2, step2);
															}break;
										case REAGENT_SMALLHOLE_CODE:{
																ConfigSetDword(CFG_REAGENT_COOL_X_POS3, step1);
																ConfigSetDword(CFG_REAGENT_COOL_Y_POS3, step2);
															}break;
										case REAGENT_LEVEL_CODE:{
																ConfigSetDword(CFG_REAGENT_CLEAN_X_POS, step1);
																ConfigSetDword(CFG_REAGENT_CLEAN_Y_POS, step2);
															}break;
										case (REAGENT_LEVEL_CODE+1):{
																ConfigSetDword(CFG_REAGENT_COOPERATE_X_POS, step1);
																ConfigSetDword(CFG_REAGENT_COOPERATE_Y_POS, step2);
															}break;
										case 271:			{  //Z轴移动到大孔垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_REAGENT_BIG_HOLE_UP_POS, step3);	//光学测量垂直位置
															}break;
										case 5391:			{  //Z轴移动到中孔垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_REAGENT_MID_HOLE_UP_POS, step3);	//光学测量垂直位置
															}break;
										case 13071:			{  //Z轴移动到小孔垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_REAGENT_SMALL_HOLE_UP_POS, step3);	//光学测量垂直位置
															}break;
										case 25871:			{  //Z轴移动到清洗池垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_REAGENT_CLEAN_UP_POS, step3);	//光学测量垂直位置
															}break;
										case 26127:			{  //Z轴移动到配合抓手垂直位置
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_REAGENT_COOPERATE_UP_POS, step3);	//光学测量垂直位置
															}break;
										default:break;  	 //发送的工作位置错误
									}
									ConfigSetDone();
									SendResultMessage();
								}break;
		case MSG_REGEHOST_MOTOR_INQUIRE:  SendResultMessage();	break;	
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_SYN_DONE:
		case MSG_TIR_DONE:
		case MSG_DET_DONE:
		case MSG_REGEHOST_MOTOR_ENABLE:
		case MSG_REGEHOST_MOTOR_RESET:		
		case MSG_REGEHOST_MOTOR_TOZERO:
		case MSG_REGEHOST_MOTOR_MOVREL:
		case MSG_REGEHOST_MOTOR_MOVABS:
		case MSG_REGEHOST_MOTOR_MOVWRK:
		case MSG_REGEHOST_MOTOR_ZAIXDET:
		case MSG_REGEHOST_MOTOR_SUBDRA:               
		case MSG_REGEHOST_MOTOR_MOVXY:
		case MSG_REGEHOST_MOTOR_ZAXISSUB:
		case MSG_REGEHOST_MOTOR_ZAXISDRA:
		case MSG_REGEHOST_MOTOR_ZAXISCLE:	RegeStateMessageHandler(msg);	break;
		case MSG_REGEHOST_MOTOR_PUMPSET:
		case MSG_REGEHOST_MOTOR_PUMPGET:
		case MSG_REGEHOST_MOTOR_VALVESET:
		case MSG_REGEHOST_MOTOR_VALVEGET:
		case MSG_REGEHOST_MOTOR_IOSET:
		case MSG_REGEHOST_MOTOR_IOGET:
		case MSG_REGEHOST_MOTOR_OPTOGET:
		case MSG_REGEHOST_MOTOR_FAULT:		break;
		case MSG_REGEHOST_LIQUID_SET:		break;
		case MSG_REGEHOST_LIQUID_WATCH:	RegeSendCmdTemp();	break;		
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
							SendResultMessage();										//发送结果码
							memset(&MainState.Parameter,0,sizeof(MainState.Parameter)); //清空数据
							LiquidneedleDisable();										//关闭外部中断
							TimerStop(); 												//关闭定时器
							TimerStop2();														
							MainState.stateManageTop = STATE0_INIT;    //接下来只能复位,注意此时除复位命令，其它指令全部返回错误
						}break;
		default:	break;//something is wrong		
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID RegeInit(VOID)
{
	UINT8 tag = DebugRegister ("rege");
	MessageRegisterHandler (RegeMessageHandler);
	DebugLog(tag, "Rege App Init.");
	
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
	MainState.motors[MOTOR_X].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_X);
	MainState.motors[MOTOR_Y].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_Y);
	MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_REAGENT_Z_DOWN);
	MainState.motors[MOTOR_SYN].speedPlan.accTable = GetAccelerateTable (ACCTBL_REAGENT_SYN_DOWN);
	
	StepMotorInit(&(MainState.motors[MOTOR_X]),MSG_X_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Y]),MSG_Y_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Z]),MSG_Z_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_SYN]),MSG_SYN_DONE );
	
	MainState.stateManageTop = STATE0_READY;
}




/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Load.c
 *******************************************************************/
#include "Load.h"
#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "SlideState.h"
#include "Host.h"
#include "Config.h"
#include "SerialDownLoad.h"
#include "boot_addr_F103ZE.h"
/********************************************************************
 *
 *******************************************************************/
STATIC UINT8 tag=0;
LoadApp loadApp;
#define VERSION_XX	1
#define VERSION_YY	0
#define VERSION_ZZ	3
/********************************************************************
 *
 *******************************************************************/
STATIC VOID SendVersionMessage(VOID)
{
	BYTE code1,code2,code3;
	BYTE respBuf[4];
	code1 = (((UINT32)VERSION) & 0x0000FF00)>>16;
	code2 = (((UINT32)VERSION) & 0x0000FF00)>>8;
	code3 = (UINT32)VERSION    & 0x000000FF;
	respBuf[0] = 0;
	respBuf[1] = code1;	
	respBuf[2] = code2;	
	respBuf[3] = code3;
	HostResponse(respBuf, 4);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID DispConfig(Message* msg)
{
	UINT16 id = (UINT16)(msg->p1);
	for (UINT32 i=id; i<id+10; i++){
		INT32 v = ConfigGet (i);
		if (i==CFG_CRC32){
			DebugLog(tag, "[CRC ] = %08X", v);
		}
		else if(i==CFG_MAGIC){
			INT32 vv[2];
			vv[0] = v;
			vv[1] = 0;
			DebugLog(tag, "[MAGI] = %8s", (CHAR*)vv);
		}
		else
			DebugLog(tag, "[%4d] = %8d", i, v);
	}
}

STATIC VOID SetConfig(Message* msg)
{
	UINT16 id = (UINT16)(msg->p1);
	INT32  v  = msg->p2;
	if (id > 0){
		if (id < 1024)
			ConfigSet(id, v);
		else
			ConfigResetDefault ();
	}
}

/********************************************************************
 *
 *******************************************************************/
STATIC INT32 MessageGetPos (INT32 id)
{
	StepMotor* motor = NULL;
	StepMotor* disp = & (loadApp.motors[MOTOR_DISP]);
	StepMotor* load = & (loadApp.motors[MOTOR_LOAD]);

	if(0 <= id && id < MOTOR_MAX )
		motor = & (loadApp.motors[id]);

	INT32 pos = 0;
	if(motor){
		pos = StepMotorGetPos(motor);
		DebugLog(tag, "Motor %d Pos: %d", id, pos);
	}
	else{
		INT32 pos1 = StepMotorGetPos (disp);
		INT32 pos2 = StepMotorGetPos (load);
		DebugLog(tag, "Motor Pos: %d, %d", pos1, pos2);
	}
	return pos;
}
/********************************************************************
 *
 *******************************************************************/
STATIC INT32 MessageGetOpto(INT32 id)
{
	if(id>=IO_OPTO1 && id<=IO_OPTO6){
		Gpio* gpio = BoardGetGpio(id);
		INT32 v = GpioGet (gpio);
		return v;
	}
	else{
		UINT16 vopto[IO_OPTO6-IO_OPTO1+1];
		Gpio* gpio;
		for(INT32 id=IO_OPTO1; id<=IO_OPTO6; id++){
			gpio = BoardGetGpio(id);
			UINT16 v = GpioGet(gpio);
			vopto[id-IO_OPTO1] = v;
		}
		DebugLog(tag, "opto: %d, %d, %d, %d, %d, %d",
			vopto[0], vopto[1], vopto[2],
			vopto[3], vopto[4], vopto[5]);
		return -1;
	}
}

/********************************************************************
 *
 *******************************************************************/
STATIC VOID MessageOnStateBasic(Message* msg)
{
	switch(msg->id){
	case MSG_LOAD_DONE:
	case MSG_DISP_DONE:
		loadApp.state0 = loadApp.stateForBack;
		break;
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MessageOnStateInit(Message* msg)
{
	switch(msg->id){
	case MSG_INIT:
		loadApp.bMotorReset[MOTOR_DISP] = FALSE;
		loadApp.bMotorReset[MOTOR_LOAD] = FALSE;
		loadApp.state0 = STATE0_RESET;
		loadApp.state1 = STATE1_RST_IDLE;
		loadApp.bDispWatchSlide = FALSE;
		loadApp.needResponse = FALSE;
		break;
	}
}

/********************************************************************
 *
 *******************************************************************/
STATIC VOID MessageOnState(Message* msg)
{
	//DebugLog(tag, "Message on State: %d", loadApp.state0);
	if(loadApp.receiveCmd == HOST_CMD_LOADCUP){   //此判断只适用调试
		switch(msg->id)
		{
			case MSG_HOST_LOADCUP:{
						INT16 wrkpos = msg->p1;
						StepMotor* motor = &(loadApp.motors[MOTOR_LOAD]);
						loadApp.needResponse = TRUE;						
						switch(wrkpos)
						{
							case 1:{	//自动装杯最底位置				
									INT32 lower = ConfigGet(CFG_LOAD_POS_VBOT);
									StepMotorMovAbs(motor, lower, LOAD_SPEED_MAX);
								}break;
							case 2:{   //自动装杯最高位置
									INT32 upper = ConfigGet(CFG_LOAD_POS_VTOP);
									StepMotorMovAbs(motor, upper, LOAD_SPEED_MAX);
								}break;
							default:break;
						}
					}break;
			case MSG_LOAD_DONE:{
						if (loadApp.needResponse){
							loadApp.needResponse = FALSE;
							ResponseState();
						}
					}break;
			default:break;
		}
		
	}
	else{
		if (msg->id == MSG_DISP_DONE){
			INT32 pos = loadApp.motors[MOTOR_DISP].pos;
			while(pos < 0) pos += DISP_ROUND_STEPS;
			pos = pos % DISP_ROUND_STEPS;
			loadApp.motors[MOTOR_DISP].pos = pos;
		}
		switch(loadApp.state0){
		case STATE0_INIT:
			MessageOnStateInit  (msg);
			break;
		case STATE0_RESET:
			MessageOnStateReset (msg);
			break;
		case STATE0_READY:
			MessageOnStateReady (msg);
			break;
		case STATE0_BASIC:
			MessageOnStateBasic (msg);
			break;
		}
	}
	
}
/********************************************************************
 *
 *******************************************************************/

STATIC VOID LoadMessageHandler(Message* msg)
{
	IAP_PROCESS_START
	DebugLog(tag, "msg(%d : %d, %d, %d, %d)",
		msg->id, msg->p1, msg->p2, msg->p3, msg->p4);
	switch(msg->id){
		case MSG_INIT:
		case MSG_DISP_DONE:
		case MSG_LOAD_DONE:
		case MSG_TIMER:
		case MSG_TIMER2:
			MessageOnState (msg);
			break;
		case MSG_CMD_MOVREL:
		case MSG_CMD_MOVABS:
		case MSG_CMD_RESET:
		case MSG_CMD_DISP:
		case MSG_CMD_LOAD:
	#ifdef LOAD_TEST_AUTOLOAD
		case MSG_TEST:
	#endif
			MessageOnState(msg);
			break;
		case MSG_CMD_POS:
			MessageGetPos(-1);
			break;
		case MSG_CMD_OPTO:
			MessageGetOpto(-1);
			break;
		case MSG_HOST_VERSION:
			SendVersionMessage();
			break;
		case MSG_HOST_GETINFO:break;
		case MSG_HOST_SETINFO:break;
		case MSG_HOST_PARINFO:{
										loadApp.codeID1 = msg->p1;
										loadApp.codeID2 = msg->p2;
										loadApp.codeID3 = msg->p3;
										loadApp.codeID4 = msg->p4;
										SendParameterMessage();
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
											if(id1 >= CFG_LOAD_POSLMT_VNEG)
												ConfigSetDword(id1, value1);
											if(id2 >= CFG_LOAD_POSLMT_VNEG)
												ConfigSetDword(id2, value2);
										}
										ResponseSuccess();
							}break;
		case MSG_HOST_WRKSAVE:{
									INT32 work = msg->p1;
									INT32 step1 = StepMotorGetPos (&(loadApp.motors[MOTOR_DISP]));
									INT32 step2 = StepMotorGetPos (&(loadApp.motors[MOTOR_LOAD]));
									switch(work)
									{
										case 1  :ConfigSetDword(CFG_LOAD_POS_VBOT, step1);break;
										case 2  :ConfigSetDword(CFG_LOAD_POS_VTOP, step1);break;
										case 258:ConfigSetDword(CFG_LOAD_POS_WRK1, step1);break;  	//工作位置2
										case 259:ConfigSetDword(CFG_LOAD_POS_WRK2, step1);break;	//工作位置3
										case 260:ConfigSetDword(CFG_LOAD_POS_WRK3, step1);break;	//工作位置4
										case 261:ConfigSetDword(CFG_LOAD_POS_WRK4, step1);break;	//工作位置5																																												
										default:break;  	 //发送的工作位置错误
									}							
									ConfigSetDone();
									ResponseSuccess();
							}break;
		case MSG_HOST_MOTOR_ENABLE:
		case MSG_HOST_MOTOR_RESET:
		case MSG_HOST_MOTOR_TOZERO:
		case MSG_HOST_MOTOR_MOVREL:
		case MSG_HOST_MOTOR_MOVABS:
		case MSG_HOST_MOTOR_MOVWRK:
		case MSG_HOST_MOTOR_GETPOS:
		case MSG_HOST_LOADCUP:
		case MSG_HOST_DISPATCH:
			MessageOnState(msg);
			break;
		case MSG_HOST_IOGET:
		case MSG_HOST_IOSET:
		case MSG_HOST_FAULT:
			break;
		case MSG_HOST_OPTOGET:
			ResponseState();
			break;
		case MSG_CMD_CONFIG:
			DispConfig(msg);
			break;
		case MSG_CMD_CFGSET:
			SetConfig(msg);
			break;
		default:
			break;
	}
	IAP_PROCESS_END
}
/********************************************************************
 *
 *******************************************************************/
STATIC BYTE GetOptoState(VOID)
{
	Gpio* gpio;
	UINT16 rst1, rst2, wrk2, wrk4, slide;

	gpio = BoardGetGpio(IO_OPTO1);
	rst1 = GpioGet(gpio) & 0x01;
	gpio = BoardGetGpio(IO_OPTO2);
	rst2 = GpioGet(gpio) & 0x01;
	gpio = BoardGetGpio(WORK2_OPTO);
	wrk2 = GpioGet(gpio) & 0x01;
	gpio = BoardGetGpio(WORK4_OPTO);
	wrk4 = GpioGet(gpio) & 0x01;

	slide = 0;
	switch(slideState.state){
	case SLIDE_UNKNOWN:
	case SLIDE_LESS_1_3:
		slide = 0;
		break;
	case SLIDE_LESS_2_3:
		slide = 1;
		break;
	case SLIDE_GREAT_2_3:
		slide = 3;
		break;
	}

	BYTE state = ((slide&0x3)<<4)|(wrk4<<3)|(wrk2<<2)|(rst2<<1)|(rst1);

	return state;
}

VOID ResponseState(VOID)
{
	StepMotor* dispMotor = &(loadApp.motors[MOTOR_DISP]);
	INT16 stepAbs = (INT16)StepMotorGetPos(dispMotor);

	UINT8 data[11];
	data[0] = data[1] = 0;
	data[2] = (UINT8)(loadApp.whichWorkHoleOn);
	data[3] = (stepAbs >= 0 ? 0 : -1);
	data[4] = HWORD(stepAbs);
	data[5] = LWORD(stepAbs);
	data[6] = 0;
	data[7] = data[8] = data[9] = 0;
	data[10] = GetOptoState();
	HostResponse (data, 11);
}

VOID ResponseSuccess(VOID)
{
	UINT8 data[1];
	data[0] = 0;
	HostResponse (data, 1);
}
/********************************************************************
 *发送查询参数的值
 *******************************************************************/
VOID SendParameterMessage(VOID)
{
	INT32 value1,value2,value3,value4;
	BYTE sendBuf[25];	
	INT16 id1 = loadApp.codeID1;
	INT16 id2 = loadApp.codeID2;
	INT16 id3 = loadApp.codeID3;
	INT16 id4 = loadApp.codeID4;
	
	if(id1 >= CFG_LOAD_POSLMT_VNEG)
		value1 = ConfigGetDword(id1);
	else
		value1 = 0;
	if(id2 >= CFG_LOAD_POSLMT_VNEG)
		value2 = ConfigGetDword(id2);
	else
		value2 = 0;
	if(id3 >= CFG_LOAD_POSLMT_VNEG)
		value3 = ConfigGetDword(id3);
	else
		value3 = 0;
	if(id4 >= CFG_LOAD_POSLMT_VNEG)
		value4 = ConfigGetDword(id4);
	else
		value4 = 0;
	sendBuf[0] = 0;   //状态单元ok
	sendBuf[1] = HBYTE (id1);
	sendBuf[2] = LBYTE (id1);
	sendBuf[3] = HBYTE (id2);
	sendBuf[4] = LBYTE (id2);
	sendBuf[5] = HBYTE (id3);
	sendBuf[6] = LBYTE (id3);
	sendBuf[7] = HBYTE (id4);
	sendBuf[8] = LBYTE (id4);
	sendBuf[9]  = HBYTE (HWORD(value1));
	sendBuf[10] = HBYTE (LWORD(value1));
	sendBuf[11] = LBYTE (HWORD(value1));
	sendBuf[12] = LBYTE (LWORD(value1));
	sendBuf[13] = HBYTE (HWORD(value2));
	sendBuf[14] = HBYTE (LWORD(value2));
	sendBuf[15] = LBYTE (HWORD(value2));
	sendBuf[16] = LBYTE (LWORD(value2));
	sendBuf[17] = HBYTE (HWORD(value3));
	sendBuf[18] = HBYTE (LWORD(value3));
	sendBuf[19] = LBYTE (HWORD(value3));
	sendBuf[20] = LBYTE (LWORD(value3));
	sendBuf[21] = HBYTE (HWORD(value4));
	sendBuf[22] = HBYTE (LWORD(value4));
	sendBuf[23] = LBYTE (HWORD(value4));
	sendBuf[24] = LBYTE (LWORD(value4));
	HostResponse(sendBuf, 25);
}
/********************************************************************
 *
 *******************************************************************/

VOID StepMotorDispCallback(VOID)
{

}
/********************************************************************
 *
 *******************************************************************/

VOID LoadBasicCommandOnStateAndBackTo(UINT32 stateBackTo, Message* msg)
{
	StepMotor* motorDisp = &(loadApp.motors[MOTOR_DISP]);
	StepMotor* motorLoad = &(loadApp.motors[MOTOR_LOAD]);
	StepMotor* motor;

	if(msg->p1 == 1)
		motor = motorDisp;
	else if (msg->p1 == 2)
		motor = motorLoad;
	else
		motor = NULL;

	/*-----------------------------------------------*/
	switch(msg->id){
	case MSG_HOST_MOTOR_ENABLE:
		// todo
		break;
	case MSG_HOST_MOTOR_TOZERO:
		// todo
		break;
	case MSG_HOST_MOTOR_MOVREL:
	case MSG_CMD_MOVREL:
		// (p1, p2, p3) = id, step, speed
		if (motor){
			StepMotorMovRel(motor, msg->p2, msg->p3);
			loadApp.state0 = STATE0_BASIC;
			loadApp.stateForBack = stateBackTo;
		}
		break;
	case MSG_HOST_MOTOR_MOVABS:
	case MSG_CMD_MOVABS:
		// (p1, p2, p3) = id, step, speed
		if (motor){
			StepMotorMovAbs(motor, msg->p2, msg->p3);
			loadApp.state0 = STATE0_BASIC;
			loadApp.stateForBack = stateBackTo;
		}
		break;
	case MSG_HOST_MOTOR_MOVWRK:
		// todo
		break;
	case MSG_HOST_MOTOR_GETPOS:
		// todo
		break;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID LoadStepMotorMovAbs(StepMotor* motor, INT32 step, UINT32 speed)
{
	INT32 pos = motor->pos;
	while(step >= DISP_ROUND_STEPS)
		step -= DISP_ROUND_STEPS;

	if (step < pos)
		StepMotorMovAbs(motor, DISP_ROUND_STEPS+step, speed);
	else
		StepMotorMovAbs(motor, step, speed);
}

/********************************************************************
 *
 *******************************************************************/

VOID LoadInit(VOID)
{
	Message msg;
	MessageRegisterHandler (LoadMessageHandler);
	tag = DebugRegister ("lod");

	ConsoleRegister ("mv",  MSG_CMD_MOVREL);
	ConsoleRegister ("mva", MSG_CMD_MOVABS);
	ConsoleRegister ("rst", MSG_CMD_RESET);
	ConsoleRegister ("dsp", MSG_CMD_DISP);
	ConsoleRegister ("ld",  MSG_CMD_LOAD);
	ConsoleRegister ("pos", MSG_CMD_POS);
	ConsoleRegister ("opto", MSG_CMD_OPTO);
	ConsoleRegister ("cfg", MSG_CMD_CONFIG);
	ConsoleRegister ("set", MSG_CMD_CFGSET);

	loadApp.motors[MOTOR_DISP].lpd   = BoardGetGpio (IO_LPD1);
	loadApp.motors[MOTOR_LOAD].lpd   = BoardGetGpio (IO_LPD2);
	loadApp.motors[MOTOR_DISP].ori   = BoardGetGpio (IO_ORI1);
	loadApp.motors[MOTOR_LOAD].ori   = BoardGetGpio (IO_ORI2);
	loadApp.motors[MOTOR_DISP].opto  = BoardGetGpio (IO_OPTO1);
	loadApp.motors[MOTOR_LOAD].opto  = BoardGetGpio (IO_OPTO2);
	loadApp.motors[MOTOR_DISP].pulse = BoardGetPulseDispatch();
	loadApp.motors[MOTOR_LOAD].pulse = BoardGetPulseLoad ();

	loadApp.motors[MOTOR_DISP].speedPlan.accTable =
			GetAccelerateTable (ACCTBL_DISK_X);
	loadApp.motors[MOTOR_LOAD].speedPlan.accTable =
			GetAccelerateTable (ACCTBL_LOAD_X);

	StepMotorInit (& (loadApp.motors[MOTOR_DISP]), MSG_DISP_DONE);
	StepMotorInit (& (loadApp.motors[MOTOR_LOAD]), MSG_LOAD_DONE);
	StepMotorDisable (& (loadApp.motors[MOTOR_DISP]));
	StepMotorDisable (& (loadApp.motors[MOTOR_LOAD]));

	StepMotorSetCallback(&(loadApp.motors[MOTOR_DISP]), StepMotorDispCallback);

	SlideStateInit();

	loadApp.state0 = STATE0_INIT;
	msg.id = MSG_INIT;
	msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
	MessagePost(&msg);
	LoadProgramStateInit();
	DebugLog(tag, "Load App Init.");
}
/********************************************************************
 * End of File
 *******************************************************************/




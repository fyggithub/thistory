/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * StateMonitor.c
 *******************************************************************/

#include "Board.h"
#include "Console.h"
#include "StateMonitor.h"
#include "Host.h"
#include "Timer.h"
#include "StateLedControl.h"
#include "SerialDownLoad.h"
#define HOST_DATA_LENGTH 10
/********************************************************************
 *
 *******************************************************************/
STATIC UINT8 tag=0;
MonitorApp monitorApp;
/********************************************************************
 *·¢ËÍ°æ±¾ºÅ
 *******************************************************************/
STATIC VOID SendVersionMessage(VOID)
{
	BYTE code1,code2,code3;
	BYTE respBuf[4];
	code1 = VERSION_XX;
	code2 = VERSION_YY;
	code3 = VERSION_ZZ;
	respBuf[0] = 0;
	respBuf[1] = code1;	
	respBuf[2] = code2;	
	respBuf[3] = code3;
	HostResponse(respBuf, 4);
}
/********************************************************************
 * GPIO -> 0,LED ON,GPIO -> 1 , LED OFF
 *******************************************************************/
VOID SamLedControl(UINT8 led)
{
	for(int i=0; i<8; i++){
		UINT8 tLed = led>>i;
		GpioSet(BoardGetGpio(i+IO_SAM_LED1),tLed);
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT8 GetSam1GPIO(VOID)
{
	UINT8 tSam = 0;
	for(int i=0; i<8; i++){
		monitorApp.mSam[i] = GpioGet(BoardGetGpio(i+IO_SAM_OPTO1));//i=0->IO_SAM_OPTO1
		tSam = tSam + (monitorApp.mSam[i]<<(7-i));
	}
	monitorApp.mSamSum = tSam;
	return ~tSam;
}
/********************************************************************
 *
 *******************************************************************/
VOID SetSam1Leds(VOID)
{
	for(int i=0; i<8; i++){
		GpioSet(BoardGetGpio(i+IO_SAM_LED1),monitorApp.mSam[i]);//BoardGetGpio(i+8)->IO_SAM_LED1
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT8 GetSam2GPIO(VOID)
{
	return 0xFF;
}
/********************************************************************
 *
 *******************************************************************/
BOOL GetCleanState(VOID)
{
	BOOL tClean = GpioGet(BoardGetGpio(IO_ClEAN_FLOAT));
	monitorApp.mIsCleanEmpty = tClean;
	return tClean;
}
/********************************************************************
 *
 *******************************************************************/

BOOL GetWasteBarryState(VOID)
{
	BOOL tWaste = GpioGet(BoardGetGpio(IO_WASTE_FLOAT));
	monitorApp.mIsWasteBarryFull = tWaste;
	return tWaste;
}
/********************************************************************
 *
 *******************************************************************/
BOOL GetDoorState(VOID)
{
	BOOL tDoor = GpioGet(BoardGetGpio(IO_WASTE_FLOAT));
	monitorApp.mIsDoorOpen = tDoor;
	return tDoor;
}
/********************************************************************
 *
 *******************************************************************/
VOID PrintCurrentStateLog(VOID)
{
	DebugWarning(monitorApp.tag,"OPT:%d",monitorApp.mSamSum);
	DebugWarning(monitorApp.tag,"LED:%d",monitorApp.mSamSum);
	DebugWarning(monitorApp.tag,"cleanEmpty:%d",monitorApp.mIsCleanEmpty);
	DebugWarning(monitorApp.tag,"wasteFull:%d",monitorApp.mIsWasteBarryFull);
	DebugWarning(monitorApp.tag,"doorOpen:%d",monitorApp.mIsDoorOpen);
}
/********************************************************************
 *
 *******************************************************************/
VOID CurrentState(UINT8* data,UINT8 len)
{
	if(len>5){
		DebugWarning(monitorApp.tag,"error length State Byte");
		return;	
	}
	data[0] = GetSam1GPIO();
	data[1] = GetSam2GPIO();
	data[2] = 0xFF;//MAG Cup is virtual,it has deleted in Protocl.
	UINT8 tWaste 	= ( 0x01 & GetWasteBarryState())<<1;
	UINT8 tClean	= 0x01 & GetCleanState();
	UINT8 tConsume	= tWaste + tClean;
	data[3]			= tConsume;
	UINT8 tDoor		= GetDoorState();
	data[4]			= 0x01 & tDoor;
}
/********************************************************************
 *
 *******************************************************************/
VOID AnswerState(VOID)
{
	UINT8 hostData[HOST_DATA_LENGTH];
	hostData[0] =0x00;
	hostData[1] = GetSam1GPIO();//FF
	hostData[2] = 0xFF;//FF
	hostData[3] = 0xFF;
	hostData[4] = 0xFC;
	hostData[5] = 0xFE;
	HostResponse(hostData,6);
	/*
	hostData[0] =0x00;
	CurrentState((UINT8*)&hostData[1],5);
	SetSam1Leds();
	*/
}
/********************************************************************
 *
 *******************************************************************/
VOID EmergencyStopSend(VOID)
{
	UINT8 data[20] = {0x00};
	data[0] = 0xE1;
	data[1] = 0xC3;	
	data[2] = 0x00;
	data[3] = 0x07;	
	data[4] = 0x00;
	data[5] = 0x01;		
	data[6] = 0x01;
	data[7] = 0x01;	
	data[8] = 0xAA;
	data[9] = 0x00;	
	data[10] = 0x00;		
	HostSend(data, 1);
}
/********************************************************************
 *
 *******************************************************************/
VOID EmergencyStopPoll(VOID)
{
	STATIC UINT8 ketDownCounts = 0;
	BOOL tIsKeyDown = GpioGet(BoardGetGpio(IO_EMERGENCY_KEY));
	if(tIsKeyDown == TRUE){
		ketDownCounts++;
		if(ketDownCounts>10){
			ketDownCounts = 0;
			Message msg;
			msg.id = MSG_HOST_EMERGENCY_STOP;
			msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
			MessagePost(&msg);
			TimerStart(100,50);
			return;
		}
		TimerStartMs(10);
		return;
	}
	TimerStartMs(10);	
	ketDownCounts = 0;
	return;
}
/********************************************************************
*
 *******************************************************************/
VOID LedControl(UINT8 p1)
{
	UINT8 tLedMethod = p1;
	switch(tLedMethod){
		case SHUTDOWN:
			StateLedTimerStart(10,10);
			break;
		case START:
			StateLedTimerStart(10,10);
			break;
		case SLEEP:
			StateLedTimerStart(10,10);
			break;
		case STANDBY:
			StateLedTimerStart(10,10);
			break;
		case MAINTAIN:
			StateLedTimerStart(10,10);
			break;
		case PROCESS:
			StateLedTimerStart(10,10);
			break;
		case PAUSE:
			StateLedTimerStart(10,10);
			break;
		case ABEND:
			StateLedTimerStart(10,10);
			break;
		case LEDMETHOD_TEST:
			StateLedTimerStart(10,20);
			break;
		default:
			break;
	}
	UINT8 hostData[HOST_DATA_LENGTH];
	hostData[0] =0x00;
	hostData[1] = 0x00;
	HostResponse(hostData,2);
}
/********************************************************************
 *
 *******************************************************************/

STATIC VOID StateMonitorMessageHandler(Message* msg)
{
	UINT8 loadProgramState = GetLoadProgramState();
	if(loadProgramState == LOADWAIT){
		switch(msg->id){
			case MSG_INIT:
				break;
			case MSG_HOST_VERSION:
				SendVersionMessage();
				break;
			case MSG_HOST_GETINFO:
				break;
			case MSG_HOST_SETINFO:
				break;
			case MSG_HOST_PARINFO:
				break;
			case MSG_TIMER:
				EmergencyStopPoll();
				break;
			case MSG_TIMER2:
				GetSam1GPIO();
				SetSam1Leds();
				TimerStartMs2(200);
				break;
			case MSG_HOST_STATE_ASK:	
				AnswerState();
				break;
			case MSG_HOST_STATE_LED_CONTROL:
				LedControl(msg->p1);
				break;
			case MSG_HOST_EMERGENCY_STOP:
				EmergencyStopSend();		
				break;

		/*TEST*/	
			case MSG_READSAM1:
				DebugWarning(monitorApp.tag,"sam1:%d",monitorApp.mSamSum);
				break;
			case MSG_READSAM2:
				DebugWarning(monitorApp.tag,"no sam");
				break;
			case MSG_READDOOR:
				GetDoorState();
				PrintCurrentStateLog();
				break;
			case MSG_READCLEAN:
				GetCleanState();
				PrintCurrentStateLog();
				break;
			case MSG_READWASTE:
				GetWasteBarryState();
				PrintCurrentStateLog();
				break;		
			case MSG_OPEN_LEDON:
				SamLedControl(0x00);
				break;
			case MSG_OPEN_LEDOFF:
				SamLedControl(0xFF);		
				break;
			case MSG_CONT1:
				LedControl(LEDMETHOD_TEST);		
				break;	
			case MSG_PSD:
				PrintSaveData();		
				break;		
			default:
				break;
		}
	}
	else{
		switch(msg->id){
			case MSG_HOST_UPDATEASK:
					SerialLoadStart();			
					break;
			case MSG_HOST_UPTRAN:
					SerialLoadStart();				
					break;
			
			default:
				//DebugWarning(monitorApp.tag,"Load program fail!");
				break;
		}	
	}
}
/********************************************************************
 *
 *******************************************************************/

VOID StateMonitorInit(VOID)
{
	MessageRegisterHandler (StateMonitorMessageHandler);
	ConsoleRegister("sam1", MSG_READSAM1);
	ConsoleRegister("sam2", MSG_READSAM2);
	ConsoleRegister("door", MSG_READDOOR);
	ConsoleRegister("clean",MSG_READCLEAN);
	ConsoleRegister("waste",MSG_READWASTE);
	ConsoleRegister("samon",  MSG_OPEN_LEDON);
	ConsoleRegister("samoff",  MSG_OPEN_LEDOFF);
	ConsoleRegister("stop",  MSG_HOST_EMERGENCY_STOP);
	ConsoleRegister("cont1",  MSG_CONT1);
	ConsoleRegister("psd",  MSG_PSD);	
	tag = DebugRegister ("Mon");
	DebugLog(tag, "Monitor App Init.");
	LoadProgramStateInit();
	SamLedControl(0x00);
	TimerStartMs(10);
	TimerStartMs2(10);

	StateLedTimerStartMs(1000);

}
/********************************************************************
 * End of File
 *******************************************************************/




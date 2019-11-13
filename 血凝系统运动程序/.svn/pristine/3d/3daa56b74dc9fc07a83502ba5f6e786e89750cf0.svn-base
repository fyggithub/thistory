/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Cool.c
 *******************************************************************/
#include <string.h>
#include "Cool.h"
#include "Console.h"
#include "Board.h"
#include "Message.h"
#include "WaveTools.h"
#include "OneWire.h"
#include "DS18B20.h"
#include "LedFans.h"
#include "Timer.h"
#include "AlgorithmPID.h"
#include "Pwm.h"
#include "Host.h"
/********************************************************************
 *
 *******************************************************************/
#define TIMER_PERIOD	(1000)

typedef struct _CoolApp{
	BOOL bSensorFound;
	BOOL bStopLog;
	BOOL bRunning;
	AlgorithmPid pid;
}CoolApp;
/********************************************************************
 *
 *******************************************************************/

STATIC CoolApp coolApp;
STATIC UINT8 tag = 0;

/********************************************************************
 *		=========  Dummy for test =============
 *******************************************************************/
#define DUMMY_WARMRATE  (0.1)
#define DUMMY_COOLRATE  (0.5)
#define DUMMY_COOLDELAY (20)
#define IDEAL_TEMP      (16)
typedef struct _Dummy{
	DOUBLE temp;
	DOUBLE pwm;
	DOUBLE pwmDelay[DUMMY_COOLDELAY];
}Dummy;
STATIC Dummy dummy;
/********************************************************************
 *
 *******************************************************************/
VOID DummyInit(VOID)
{
	memset(dummy.pwmDelay, 0, sizeof(dummy.pwmDelay));
	dummy.pwm = 0.0;
	dummy.temp = 25.0;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE DummyGetTemp(VOID)
{
	DOUBLE pwm = dummy.pwmDelay[0];
	memmove(dummy.pwmDelay, &(dummy.pwmDelay[1]),
		(DUMMY_COOLDELAY-1)*sizeof(dummy.pwmDelay[0]));
	dummy.pwmDelay[DUMMY_COOLDELAY-1] = 0;
	dummy.temp -= pwm*DUMMY_COOLRATE;
	if(dummy.temp < 25.0)
		dummy.temp += DUMMY_WARMRATE;
	return dummy.temp;
}
/********************************************************************
 *
 *******************************************************************/
VOID DummySetPwm(DOUBLE pwm)
{
	dummy.pwmDelay[DUMMY_COOLDELAY-1] = pwm;
}
/********************************************************************
 *		=========  Dummy for test =============
 *******************************************************************/

/********************************************************************
 *
 *******************************************************************/
STATIC VOID DispCoeff(VOID)
{
	DOUBLE temp = TempSampleRead();//DummyGetTemp ();
	DebugLog(tag, "\r\ntemp=%.2f, pwm=%d\r\n"
		"kp = %.4f, ki = %.4f, kd = %.4f,\r\n"
		"eps1 = %.4f, eps2 = %.4f, umax = %.4f, umin = %.4f",
		temp, coolApp.bRunning,
		coolApp.pid.cKp, coolApp.pid.cKi, coolApp.pid.cKd,
		coolApp.pid.cEps1, coolApp.pid.cEps2,
		coolApp.pid.cUmax, coolApp.pid.cUmin);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID DispPidLog(DOUBLE ek, DOUBLE pwm)
{
	DebugLog(tag,"%8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f",
		ek, coolApp.pid.pitem, coolApp.pid.iitem, coolApp.pid.ditem,
		coolApp.pid.uk, pwm);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID DispHelp(VOID)
{
	DebugLog(tag, "\r\n"
		"?  - help msg\r\n"
		"q  - stop log\r\n"
		"r  - run pwm\r\n"
		"x  - break pwm\r\n"
		"k? - show coeff\r\n"
		"k[p|i|d] 1000x - set coeff"
	);
}

/********************************************************************
 *
 *******************************************************************/
VOID SetPwmDutyRatio(DOUBLE ratio)
{
	PwmStop();
	PwmSetDutyRatio ((UINT32)(ratio*1000));
	PwmStart();
}

/********************************************************************
 *
 *******************************************************************/
STATIC VOID MessageOnTimer(VOID)
{
	DOUBLE temp = TempSampleRead();//DummyGetTemp ();
	DOUBLE ek = temp-IDEAL_TEMP;
    TempSampleStart();//
	DOUBLE pwm = PIDCalc (&(coolApp.pid), ek);
	if(coolApp.bRunning){
		//DummySetPwm (pwm);
		SetPwmDutyRatio (pwm);
		if(!coolApp.bStopLog)
			DispPidLog (ek, pwm);
	}
	else{
		//DummySetPwm(0);
		SetPwmDutyRatio (0);
	}

//	UINT8 wave = (UINT8)(temp*5);
//	WaveData (wave);
}
/********************************************************************
 *
 *******************************************************************/

STATIC VOID CoolMessageHandler(Message* msg)
{
    UINT8 data[10];
    DOUBLE temp = 0;
    switch(msg->id){
	case MSG_HOST_VERSION:
		data[0] = 0x00;
		data[1] = (VERSION & 0xFF0000)>>16;
		data[2] = (VERSION & 0x00FF00)<<8>>16;
		data[3] = (VERSION & 0x0000FF)<<8>>16;
		HostResponse(data,4);
		break;
	case MSG_TIMER:
		MessageOnTimer();
		TimerStart(TIMER_PERIOD, 5);
		break;
	case MSG_TIMER2:
		coolApp.bStopLog = FALSE;
		break;
    case MSG_ENABLE_COOL:
        if(msg->p1 == 0){
            TimerStop();
			data[0] =0x00;
			HostResponse(data,1);
		}
        else{
            TimerStart(TIMER_PERIOD, 5);
			data[0] =0x00;
			HostResponse(data,1);
		}	
        break;
    case MSG_ASK_TEMPERATURE:
         temp = TempSampleRead()*100;
         data[0] =((INT16) temp )>>8;
         data[1] = (INT16)temp;
		/*tiaoshi*/
         data[0] =0x00;
         data[1] =0x06;	
		data[2] =0x40;
         HostResponse(data,3);
        break;
	case MSG_STOPLOG:
		coolApp.bStopLog = TRUE;
		TimerStart2 (1000, 10);
		break;
	case MSG_RUN:
		coolApp.bRunning = TRUE;
		DebugLog(tag, "PWM Opened.");
		break;
	case MSG_BREAK:
		coolApp.bRunning = FALSE;
		DebugLog(tag, "PWM is OFF Now!!!");
		break;
	case MSG_HELP:
		DispHelp();
		break;
	case MSG_KPSET:
		coolApp.pid.cKp = (msg->p1)/1000.0;
		DispCoeff();
		break;
	case MSG_KISET:
		coolApp.pid.cKi = (msg->p1)/1000.0;
		DispCoeff();
		break;
	case MSG_KDSET:
		coolApp.pid.cKd = (msg->p1)/1000.0;
		DispCoeff();
		break;
	case MSG_KDISP:
		DispCoeff();
		break;
	default:
		break;
	}
}
/********************************************************************
 *
 *******************************************************************/

VOID CoolInit(VOID)
{
	//// MessageHandler
	tag = DebugRegister ("col");
	MessageRegisterHandler (CoolMessageHandler);

	//// Console Command
	ConsoleRegister("ki", MSG_KISET);
	ConsoleRegister("kd", MSG_KDSET);
	ConsoleRegister("kp", MSG_KPSET);
	ConsoleRegister("k?", MSG_KDISP);
	ConsoleRegister("q",  MSG_STOPLOG);
	ConsoleRegister("r",  MSG_RUN);
	ConsoleRegister("x",  MSG_BREAK);
	ConsoleRegister("?",  MSG_HELP);

	//// Init Led, WaveTools, 1Wire, Sensor
	TimerInit();
	//DummyInit();
	LED1Set(LED_OFF);
	LED2Set(LED_OFF);
	//WaveInit();
	PwmInit();

	coolApp.bStopLog = FALSE;
	coolApp.bRunning = TRUE;
	coolApp.bSensorFound = OneWireInit();
	if (coolApp.bSensorFound)
		TempSampleInfo();

	//// Init Coeff
	coolApp.pid.cKp = 6;
	coolApp.pid.cKi = 0.6;
	coolApp.pid.cKd = 12.0;//3 xshell ÐÞ¸Ä
	coolApp.pid.cEps1 = 0.6;
	coolApp.pid.cEps2 = 0.3;
	coolApp.pid.cUmax = 1.0;
	coolApp.pid.cUmin = 0.0;
	PIDInit (&(coolApp.pid));

	
	//// Finished
    TempSampleStart();//
	TimerStart(TIMER_PERIOD, 1);
	DebugLog(tag, "Cool App Init Finished.");
}

/********************************************************************
 * END
 *******************************************************************/


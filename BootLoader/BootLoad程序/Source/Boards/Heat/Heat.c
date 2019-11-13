/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Cool.c
 *******************************************************************/
#include <string.h>
#include "Heat.h"
#include "Console.h"
#include "Board.h"
#include "Message.h"
#include "WaveTools.h"
#include "TLC2543.h"
#include "Led.h"
#include "Timer.h"
#include "AlgorithmPID.h"
#include "Pwm.h"
#include "Host.h"
/********************************************************************
 *
 *******************************************************************/
#define TIMER_PERIOD	(20)
#define IDEAL_TEM       (37)
STATIC UINT8 STATUS=0;


typedef struct _HeatApp{
	BOOL bSensorFound;
	BOOL bStopLog;
	BOOL bRunning;
	AlgorithmPid pid;
    UINT8 adTimes;
    DOUBLE temp;
}HeatApp;
/********************************************************************
 *
 *******************************************************************/

STATIC HeatApp heatApp;
STATIC UINT8 tag = 0;

/********************************************************************
 *		=========  Dummy for test =============
 *******************************************************************/
#define DUMMY_WARMRATE  (0.1)
#define DUMMY_COOLRATE  (0.5)
#define DUMMY_COOLDELAY (20)
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
		temp, heatApp.bRunning,
		heatApp.pid.cKp, heatApp.pid.cKi, heatApp.pid.cKd,
		heatApp.pid.cEps1,heatApp.pid.cEps2,
		heatApp.pid.cUmax, heatApp.pid.cUmin);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID DispPidLog(DOUBLE ek, DOUBLE pwm)
{
	DebugLog(tag,"%8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f",
		ek, heatApp.pid.pitem, heatApp.pid.iitem, heatApp.pid.ditem,
		heatApp.pid.uk, pwm, heatApp.temp);
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
	DOUBLE temp = TempSampleRead();
    heatApp.adTimes++;
    heatApp.temp=temp;
    if(heatApp.adTimes>=9){
        LED1Set(STATUS);
        STATUS = (STATUS==0) ? 1: 0;
        heatApp.adTimes=0;
        DOUBLE ek  = IDEAL_TEM-temp;
        DOUBLE pwm = PIDCalc (&(heatApp.pid), ek);
        if(heatApp.bRunning){
            SetPwmDutyRatio (pwm);
            if(!heatApp.bStopLog)
                DispPidLog (ek, pwm);
        }
        else{
            SetPwmDutyRatio (0.5);
        }
//        UINT8 wave = (UINT8)(temp*5);
//        WaveData (wave);
        
    }
    
}
/********************************************************************
 *
 *******************************************************************/

STATIC VOID HeatMessageHandler(Message* msg)
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
			heatApp.bStopLog = FALSE;
			break;
		case MSG_STOPLOG:
			heatApp.bStopLog = TRUE;
			TimerStart2 (1000, 10);
			break;
		case MSG_ENABLE_HEAT:
			if(msg->p1 == 0){
				TimerStop();
			}
			else
				TimerStart(TIMER_PERIOD, 5);
			break;
		case MSG_ASK_TEMPERATURE:
			 temp = TempSampleRead()*100;
			 data[0] =((INT16) temp )>>8;
			 data[1] = (INT16)temp;
			 HostResponse(data,2);
			break;
		case MSG_RUN:
			heatApp.bRunning = TRUE;
			DebugLog(tag, "PWM Opened.");
			break;
		case MSG_BREAK:
			heatApp.bRunning = FALSE;
			DebugLog(tag, "PWM is OFF Now!!!");
			break;
		case MSG_HELP:
			DispHelp();
			break;
		case MSG_KPSET:
			heatApp.pid.cKp = (msg->p1)/1000.0;
			DispCoeff();
			break;
		case MSG_KISET:
			heatApp.pid.cKi = (msg->p1)/1000.0;
			DispCoeff();
			break;
		case MSG_KDSET:
			heatApp.pid.cKd = (msg->p1)/1000.0;
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

VOID HeatInit(VOID)
{
	//// MessageHandler
	tag = DebugRegister ("het");
	MessageRegisterHandler (HeatMessageHandler);

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
    TLCInit();
	LED1Set(LED_OFF);
	LED2Set(LED_OFF);
//	WaveInit();
	PwmInit();

	heatApp.bStopLog = FALSE;
	heatApp.bRunning = TRUE;
    heatApp.adTimes  =0;


	heatApp.pid.cKp = 0.6;//6
	heatApp.pid.cKi = 0.005;//0.05
	heatApp.pid.cKd = 0;//3 xshell ÐÞ¸Ä
	heatApp.pid.cEps1 = 0.5;
	heatApp.pid.cEps2 = -0.5;
	heatApp.pid.cUmax = 0.7;//0.99
	heatApp.pid.cUmin = 0;

	PIDInit (&(heatApp.pid));
	TimerStart(TIMER_PERIOD, 5);
	DebugLog(tag, "Heat App Init Finished.");
}

/********************************************************************
 * END
 *******************************************************************/


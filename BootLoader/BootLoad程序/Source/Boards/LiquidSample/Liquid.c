/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Liquid.c
 *******************************************************************/
#include <string.h>
#include "Liquid.h"
#include "Console.h"
#include "Board.h"
#include "Message.h"
#include "MCP41010T.h"
#include "Led.h"
#include "Timer.h"
#include "LiquidPwm.h"
#include "StateLiquid.h"
#include "Bits.h"
#include "HostLiquid.h"
#include "Tim4.h"
#include "Adc.h"
#include "Host.h"
#include "delayus.h"
/********************************************************************
 *
 *******************************************************************/
#define TIMER_PERIOD	(1)
#define TIMER_PERIOD_PRINT	(10)
//STATIC UINT8 STATUS=0;
STATIC UINT8 tag = 0;
ntcApp NtcApp;
liqApp LiqApp;

STATIC VOID DispCoeff(VOID)
{
	DOUBLE temp = ReadNtcValue();//DummyGetTemp ();
	DebugLog(tag, "\r\ntemp=%.2f, pwm=%d\r\n"
    "kp = %.4f, ki = %.4f, kd = %.4f,\r\n"
    "eps1 = %.4f, eps2 = %.4f, umax = %.4f, umin = %.4f",
    temp, NtcApp.bRunning,
    NtcApp.pid.cKp, NtcApp.pid.cKi, NtcApp.pid.cKd,
    NtcApp.pid.cEps1,NtcApp.pid.cEps2,
    NtcApp.pid.cUmax, NtcApp.pid.cUmin);
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
 *adj increate as res increate
 *******************************************************************/
VOID LiqAdjust(VOID)
{
    DOUBLE res=0;
//    LiqApp.res =128;
//    setCD4024Res(LiqApp.res);
//    DebugLog(tag,"Res:%4.2f,val:%4.2f",res,ReadLqAdjValue());
//    LiqApp.adjustRunning=TRUE;
   
    while(LiqApp.adjustRunning ==FALSE){       
        res =  (DOUBLE)(LiqApp.res)/256*10;    
        if(ReadLqAdjValue()<ADJUSTMIN)
            LiqApp.res++;                
        else if(ReadLqAdjValue()>ADJUSTMAX) {
            if(LiqApp.res==0)
                LiqApp.res=0;
            else
                LiqApp.res--;
        }
 //LiqApp.res=(LiqApp.res==0)? 0 : (LiqApp.res--);
        else
            LiqApp.adjustTimes++;
        setCD4024Res(LiqApp.res);
		for(UINT8 i=0; i<50; i++){
			DebugLog(tag,"Res:%4.2f,val:%4.2f",res,ReadLqAdjValue());
		}
        //DebugLog(tag,"Res:%4.2f,val:%4.2f",res,ReadLqAdjValue());
        if(LiqApp.adjustTimes>ADJUSTTIMES)
        LiqApp.adjustRunning=TRUE;
    }
   
}
/********************************************************************
 *
 *******************************************************************/

VOID LiquidProcess(VOID)
{   
    if(IsLiquidContact() == TRUE){
        if(GpioGet(BoardGetGpio(F2LIQUID)) == 1){
            //DelayUS(2000);
            GpioSet(BoardGetGpio(L2FOURAXIS),1);
            return;
        }
        GpioSet(BoardGetGpio(L2FOURAXIS),0);
    }
    else{
        GpioSet(BoardGetGpio(L2FOURAXIS),1);
    }
}
STATIC VOID DispPidLog(VOID)
{
	DOUBLE temp0 = ReadNtcValue();
    
    DebugLog(tag,"%d,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f",
		NtcApp.state,temp0,NtcApp.pid.pitem, NtcApp.pid.iitem, NtcApp.pid.ditem,
		NtcApp.pid.uk, NtcApp.pwm);
}

/********************************************************************
 *
 *******************************************************************/
STATIC VOID HeatMessageHandler(Message* msg)
{
    UINT8 data[2];
    DOUBLE temp0;
    static int j=0;
    switch(msg->id){
	case MSG_TIMER:
		if(NtcApp.state == NTC_STATE_READY){
           // temp0 = NtcStateReady();
            DispPidLog();
            TimerStart(TIMER_PERIOD, 10);
        }
		break;
	case MSG_TIMER2:
        if(NtcApp.state == NTC_STATE_RUN){
          //  temp0 = NtcStateStop();
            DispPidLog();
            TimerStart2(TIMER_PERIOD, NtcApp.stopTime);
        }
        else if(NtcApp.state == NTC_STATE_STOP){
          //  temp0 = NtcStateReady();
           DispPidLog();
            TimerStart(TIMER_PERIOD, 10);
        }
		break;
    case MSG_NTC_TEMPERATURE:       
        data[0] = HBYTE(ReadNtcValueUint16());
        data[1] = LBYTE(ReadNtcValueUint16());
        HostResponse(data,2);
        break;
    case MSG_NTC_RUN:
	
        NtcApp.state =  NTC_STATE_RUN;
        temp0 = NtcStateRun(msg->p1);
        DispPidLog();
        NtcApp.stopTime = msg->p3;
		TimerStart2 (TIMER_PERIOD, msg->p2);
        break;
    case MSG_NTC_READY:
       // temp0 = NtcStateReady();
        DispPidLog();
        TimerStart(TIMER_PERIOD, 10);
        break;        
    case MSG_NTC_STOP:
        NtcApp.state =  NTC_STATE_STOP;
      //  temp0 =  NtcStateStop();
       DispPidLog();
	case MSG_STOPLOG:
		NtcApp.bStopLog = TRUE;
		TimerStart2 (1000, 10);
		break;
	case MSG_RUN:
		NtcApp.bRunning = TRUE;
		DebugLog(tag, "PWM Opened.");
		break;
	case MSG_STOP:
		NtcApp.bRunning = FALSE;
		DebugLog(tag, "PWM is OFF Now!!!");
		break;
	case MSG_HELP:
		DispHelp();
        LED2Set(LED_OFF);
		break;
	case MSG_KPSET:
		NtcApp.pid.cKp = (msg->p1)/1000.0;
		DispCoeff();
		break;
	case MSG_KISET:
		NtcApp.pid.cKi = (msg->p1)/1000.0;
		DispCoeff();
		break;
	case MSG_KDSET:
		NtcApp.pid.cKd = (msg->p1)/1000.0;
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
VOID LiquidInit(VOID)
{
	//// MessageHandler
	tag = DebugRegister ("het");
	MessageRegisterHandler (HeatMessageHandler);

	ConsoleRegister("ki", MSG_KISET);
	ConsoleRegister("kd", MSG_KDSET);
	ConsoleRegister("kp", MSG_KPSET);
	ConsoleRegister("k?", MSG_KDISP);
	ConsoleRegister("q",  MSG_STOPLOG);
	ConsoleRegister("r",  MSG_RUN);
	ConsoleRegister("x",  MSG_STOP);
	ConsoleRegister("?",  MSG_HELP);

	//// Init Led, WaveTools, 1Wire, Sensor
	TimerInit();
    //TLCInit();
	LED1Set(LED_OFF);
	LED2Set(LED_OFF);
//	WaveInit();
    PwmInit();
    PwmSetDutyRatio(0);
    AdcValueInit();
	NtcApp.bStopLog = FALSE;
	NtcApp.bRunning = TRUE;
    NtcApp.state = NTC_STATE_READY;
	NtcApp.pid.cKp = 3;//0.6
	NtcApp.pid.cKi = 0.001;//0.01;//0.005
	NtcApp.pid.cKd = 0.6;//3 xshell ÐÞ¸Ä
	NtcApp.pid.cEps1 = 0.05;
	NtcApp.pid.cEps2 = 0.05;
	NtcApp.pid.cUmax = 0.95;
	NtcApp.pid.cUmin = 0;
//setCD4024Res(128);
	PIDInit (&(NtcApp.pid));
	TimerStart(TIMER_PERIOD, 10);
    TimerStart2(TIMER_PERIOD, 10);
    LiqApp.adjustRunning = FALSE;

    LiqAdjust();
	DebugLog(tag, "Liquit App Init Finished.");   
}

/********************************************************************
* END
*******************************************************************/

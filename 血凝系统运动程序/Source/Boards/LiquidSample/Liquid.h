/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Liquid.h
 *******************************************************************/
#ifndef _HEAT_H_
#define _HEAT_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
#include "AlgorithmPID.h"
/********************************************************************
 *
 *******************************************************************/
#define LIQTIMES        (10)
#define THREHOLD        (0.6)
#define ADJUSTMIN       (1.6)
#define ADJUSTMAX       (1.7)
#define ADJUSTTIMES     (2)
/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_KPSET = 100,
	MSG_KISET,
	MSG_KDSET,
	MSG_KDISP,
	MSG_STOPLOG,
	MSG_HELP,
	MSG_RUN,
	MSG_STOP,
    MSG_NTC_RUN,
    MSG_NTC_READY,
    MSG_NTC_STOP,
    MSG_NTC_TEMPERATURE,
};
/********************************************************************
 *
 *******************************************************************/
enum{
    NTC_STATE_STOP,
    NTC_STATE_READY,
    NTC_STATE_RUN
};
/********************************************************************
 *
 *******************************************************************/
enum{
    CLEAN = 0,
    REAGENT,
};
 /********************************************************************
 *
 *******************************************************************/
typedef struct _NtcApp{
	BOOL bStopLog;
	BOOL bRunning;
    UINT8 state;
    UINT8 mode;
	AlgorithmPid pid;
    UINT16 remTim2;
    UINT16 stopTime;
    DOUBLE pwm;
}ntcApp;
/********************************************************************
 *
 *******************************************************************/
typedef struct _LiqApp{
	BOOL    bRunning;
    BOOL    adjustRunning;
	UINT8   res;
    UINT8   contactTimes;
    UINT8   adjustTimes;
    DOUBLE  value;   
}liqApp;
/********************************************************************
 *
 *******************************************************************/
extern ntcApp NtcApp;
extern liqApp LiqApp;
VOID LiquidInit(VOID);
VOID LiquidProcess(VOID);
VOID SetPWMc(DOUBLE K1,DOUBLE Ta,DOUBLE t1);
VOID SetPWMr(DOUBLE K2,DOUBLE V,DOUBLE t2);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // 



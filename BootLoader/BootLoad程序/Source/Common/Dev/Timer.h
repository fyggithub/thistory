/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Timer.h
 *******************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
/********************************************************************
 *
 *******************************************************************/
VOID TimerInit(VOID);
VOID TimerStartMs(UINT16 ms);
VOID TimerStart(UINT16 ms, UINT16 times);
VOID TimerStop(VOID);

VOID TimerInit2(VOID);
VOID TimerStartMs2(UINT16 ms);
VOID TimerStart2(UINT16 ms, UINT16 times);
VOID TimerStop2(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _TIMER_H_

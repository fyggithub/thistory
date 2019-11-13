/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时
 * StateLedControl.h
 *******************************************************************/
#include "Datatypes.h"
VOID StateLedTimerStartMs(UINT16 ms);
VOID StateLedTimerStart(UINT16 ms, UINT16 times);
VOID StateLedTimerStop(VOID);
VOID STATE_TIMER_IRQHANDLE(VOID);
/********************************************************************
 *
*******************************************************************/

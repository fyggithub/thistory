/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Timer.h
 *******************************************************************/
#ifndef _TIMER_PD_H_
#define _TIMER_PD_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
/********************************************************************
 *
 *******************************************************************/
#define TIMER3_CLK_ENABLE()  __HAL_RCC_TIM3_CLK_ENABLE()

VOID TimPdInit(VOID);
VOID TimPdStart(VOID);
VOID TimDetectSet(UINT32 mode,UINT32 nothing,UINT32 cleanup);
VOID TimLedSet(UINT32 p1);
VOID TimeAsk(VOID);
UINT32 getTIM3time(VOID);
UINT16 getTIM3cnt(VOID);
VOID setTIM3time(VOID);
VOID tim3start(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _TIMER_H_

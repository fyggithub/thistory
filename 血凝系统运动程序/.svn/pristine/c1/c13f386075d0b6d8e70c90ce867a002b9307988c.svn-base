/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SlideState.h
 *******************************************************************/
#ifndef _SLIDE_STATE_H_
#define _SLIDE_STATE_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
#include "Board.h"
/********************************************************************
 *
 *******************************************************************/
enum {
	SLIDE_UNKNOWN = 0, SLIDE_LESS_1_3,
	SLIDE_LESS_2_3, SLIDE_GREAT_2_3,
	SLIDE_STATE_MAX,
};

typedef struct _SlideState{
	INT32  startCnt;
	BOOL   isCancel;
	UINT32 state;
	BOOL   detectedUpper;
	BOOL   detectedLower;
	UINT16 cntUpper;
	UINT16 cntLower;
	TIM_HandleTypeDef timerDef;
}SlideState;
extern SlideState slideState;
/********************************************************************
 *
 *******************************************************************/
VOID SlideStateInit(VOID);
VOID SlideStateWatchStart(VOID);
VOID SlideStateWatchStop(VOID);
VOID SlideStateUpdateAsLoaded(VOID);
VOID SlideStateUpdateAsDispatched(VOID);
/********************************************************************
 *
 *******************************************************************/
#endif // _SLIDE_STATE_H_

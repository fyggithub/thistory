/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SlideState.c
 *******************************************************************/
#include "SlideState.h"
#include "Console.h"
/********************************************************************
 *
 *******************************************************************/

// system clock is 66355200 Hz
#define PRESCALER		(1296-1) // 51.2 clk/ms

SlideState slideState;
/********************************************************************
 *
 *******************************************************************/

VOID SlideStateInit(VOID)
{
	slideState.state = SLIDE_UNKNOWN;
	slideState.detectedLower = 0; // FALSE
	slideState.detectedUpper = 0; // FALSE
	slideState.cntUpper = 0;
	slideState.cntLower = 0;

	slideState.startCnt = 0;
	slideState.isCancel = FALSE;

	// 100us timer
	UINT32 period = 5;

	SLIDE_DETECT_CLK_ENABLE ();
	slideState.timerDef.Instance = SLIDE_DETECT_IRQ_TIM;
	slideState.timerDef.Init.Prescaler = PRESCALER;
	slideState.timerDef.Init.CounterMode = TIM_COUNTERMODE_UP;
	slideState.timerDef.Init.Period = period;

	HAL_NVIC_SetPriority (SLIDE_DETECT_IRQN, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ (SLIDE_DETECT_IRQN);
	__HAL_TIM_CLEAR_IT (&(slideState.timerDef), TIM_IT_UPDATE);
}
/********************************************************************
 *
 *******************************************************************/

VOID SlideStateWatchStart(VOID)
{
	if (slideState.startCnt == 0) {
		slideState.detectedLower = 0;
		slideState.detectedUpper = 0;
		slideState.cntUpper = 0;
		slideState.cntLower = 0;
		HAL_TIM_Base_Start_IT (&slideState.timerDef);
	}
	else{
		slideState.isCancel = TRUE; // conflict
	}

	slideState.startCnt ++;
}
/********************************************************************
 *
 *******************************************************************/

VOID SlideStateWatchStop(VOID)
{
	if (slideState.startCnt == 1)
		HAL_TIM_Base_Stop_IT (&slideState.timerDef);
	slideState.startCnt --;
}
/********************************************************************
 *	Upper      Lower
 *	Changed    Changed
 *
 *     0         0          --  /
 *     0         1          --  /
 *     1         0          --  Nearly Full
 *     1         1          --  Nearly Empty
 *
 *******************************************************************/
VOID SlideStateUpdateAsLoaded(VOID)
{
	if (slideState.startCnt == 0){
		if (slideState.isCancel == FALSE ) {
			if(slideState.detectedUpper){
				if(slideState.detectedLower)
					slideState.state = SLIDE_LESS_1_3;
				else
					slideState.state = SLIDE_LESS_2_3;
			}
		}
		else{
			slideState.isCancel = FALSE;
		}
	}

	DebugLog(0, "slide: %d, %d, %d", slideState.detectedUpper,
		slideState.detectedLower, slideState.state);
}
/********************************************************************
 *	Upper      Lower
 *	Changed    Changed
 *
 *     0         0          -- /
 *     0         1          -- Nearly Empty
 *     1         0          -- /
 *     1         1          -- Nearly Full
 *
 *******************************************************************/

VOID SlideStateUpdateAsDispatched(VOID)
{
	if (slideState.startCnt == 0){
		if (slideState.isCancel == FALSE ) {
			if(slideState.detectedLower){
				if(slideState.detectedUpper)
					slideState.state = SLIDE_GREAT_2_3;
				else
					slideState.state = SLIDE_LESS_2_3;
			}
			else{
				slideState.state = SLIDE_LESS_1_3;
			}
		}
		else{
			slideState.isCancel = FALSE;
		}
	}

	DebugLog(0, "slide: %d, %d, %d", slideState.detectedUpper,
		slideState.detectedLower, slideState.state);
}
/********************************************************************
 *
 *******************************************************************/

/********************************************************************
 *
 *******************************************************************/
VOID SLIDE_DETECT_IRQ_HANDLE(VOID)
{
	TIM_HandleTypeDef *htim = &(slideState.timerDef);
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);

	HAL_TIM_IRQHandler (htim);

	if(flag){
		Gpio* upper = BoardGetGpio(SLIDE_DETECT_UPPERIO);
		Gpio* lower = BoardGetGpio(SLIDE_DETECT_LOWERIO);
		UINT16 upv = GpioGet(upper);
		UINT16 lov = GpioGet(lower);

		if(upv==1)	slideState.cntUpper++;
		else		slideState.cntUpper = 0;

		if(lov==1) 	slideState.cntLower++;
		else		slideState.cntLower = 0;

		if(slideState.cntUpper >= 5){
			slideState.detectedUpper = TRUE;
		}

		if(slideState.cntLower >= 5){
			slideState.detectedLower = TRUE;
		}
	}
}
/********************************************************************
 *
 *******************************************************************/




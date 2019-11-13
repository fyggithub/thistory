/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时1 用的是定时器7，定时2用的是定时器6
 * Timer.c
 *******************************************************************/
#include "Timer.h"
#include "Board.h"
#include "Message.h"
#include "Console.h"

#ifdef STM32F051x8
#include "stm32f0xx_hal.h"
#else
#include "stm32f1xx_hal.h"
#endif
/********************************************************************
 *
 *******************************************************************/

// Default Defined For 4Axis Board, To Redefine in Board.h to Change
// System clock is 66355200 Hz As 4Axis Board
#ifndef TIMER_PRESCALER
#define TIMER_PRESCALER		(1296-1) // 51.2 clk/ms
#endif

#ifndef TIMER_PERIOD_BASE
#define TIMER_PERIOD_BASE	(512)    // Clock Period for 10ms
#endif

#if defined(STM32F103xB) || defined(STM32F051x8)

#define TIMER1_CLK_ENABLE() __HAL_RCC_TIM3_CLK_ENABLE()
#define TIMER1_TIM			TIM3
#define TIMER1_IRQN			TIM3_IRQn

#define TIMER2_CLK_ENABLE() __HAL_RCC_TIM2_CLK_ENABLE()
#define TIMER2_TIM			TIM2
#define TIMER2_IRQN			TIM2_IRQn

#define TIMER1_IRQHANDLE	TIM3_IRQHandler
#define TIMER2_IRQHANDLE	TIM2_IRQHandler

#else

#define TIMER1_CLK_ENABLE() __HAL_RCC_TIM7_CLK_ENABLE()
#define TIMER1_TIM			TIM7
#define TIMER1_IRQN			TIM7_IRQn

#define TIMER2_CLK_ENABLE() __HAL_RCC_TIM6_CLK_ENABLE()
#define TIMER2_TIM			TIM6
#define TIMER2_IRQN			TIM6_IRQn

#define TIMER1_IRQHANDLE	TIM7_IRQHandler
#define TIMER2_IRQHANDLE	TIM6_IRQHandler

#endif

STATIC TIM_HandleTypeDef timerDef;
STATIC TIM_HandleTypeDef timerDef2;
STATIC UINT16 timerTimes = 0,timerTimes2 = 0;
/********************************************************************
 *
 *******************************************************************/
VOID TimerInit(VOID)
{
	DebugLog(0, "Timer Init Finished.");
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerInit2(VOID)
{
	DebugLog(0, "Timer2 Init Finished.");
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStartMs(UINT16 ms)
{
	ms = ms > 1200 ? 1200 : ms;
	UINT32 period = ms*TIMER_PERIOD_BASE/10;

	TIMER1_CLK_ENABLE();
	timerDef.Instance         = TIMER1_TIM;
	timerDef.Init.Prescaler   = TIMER_PRESCALER;
	timerDef.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef.Init.Period      = period;
	HAL_TIM_Base_Init (&timerDef);

	HAL_NVIC_SetPriority (TIMER1_IRQN, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (TIMER1_IRQN);
	__HAL_TIM_CLEAR_IT(&timerDef, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT (&timerDef);
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStartMs2(UINT16 ms)
{
	ms = ms > 1200 ? 1200 : ms;
	UINT32 period = ms*TIMER_PERIOD_BASE/10;

	TIMER2_CLK_ENABLE ();
	timerDef2.Instance         = TIMER2_TIM;
	timerDef2.Init.Prescaler   = TIMER_PRESCALER;
	timerDef2.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef2.Init.Period      = period;
	HAL_TIM_Base_Init (&timerDef2);

	HAL_NVIC_SetPriority (TIMER2_IRQN, TIMER_NVIC_PRIOR, 1);
	HAL_NVIC_EnableIRQ   (TIMER2_IRQN);
	__HAL_TIM_CLEAR_IT(&timerDef2, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT (&timerDef2);
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStart(UINT16 ms, UINT16 times)
{
	timerTimes = times > 0 ? times-1: 0 ;
	TimerStartMs(ms);
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStart2(UINT16 ms, UINT16 times)
{
	timerTimes2 = times > 0 ? times-1: 0 ;
	TimerStartMs2(ms);
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStop(VOID)
{
	TimerStartMs(1);
    HAL_TIM_Base_Stop_IT (&timerDef);
	timerTimes = 0;
}
/********************************************************************
 *
 *******************************************************************/
VOID TimerStop2(VOID)
{
	TimerStartMs2(1);
	HAL_TIM_Base_Stop_IT (&timerDef2);
	timerTimes2 = 0;
}
/********************************************************************
 *
 *******************************************************************/
VOID TIMER1_IRQHANDLE(VOID)
{
	TIM_HandleTypeDef *htim = &timerDef;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_IRQHandler (&timerDef);
	if (flag) {
		if(!timerTimes){

			Message msg;
			HAL_TIM_Base_Stop_IT (&timerDef);
			msg.id = MSG_TIMER;
			msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
			MessagePost(&msg);
		}else{
			timerTimes--;
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID TIMER2_IRQHANDLE(VOID)
{
	TIM_HandleTypeDef *htim = &timerDef2;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_IRQHandler (&timerDef2);
	if (flag) {
		if(!timerTimes2){
			Message msg;
			HAL_TIM_Base_Stop_IT (&timerDef2);
			msg.id = MSG_TIMER2;
			msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
			MessagePost(&msg);
		}else{
			timerTimes2--;
		}
	}
}


/********************************************************************
 * END OF FILE
 *******************************************************************/


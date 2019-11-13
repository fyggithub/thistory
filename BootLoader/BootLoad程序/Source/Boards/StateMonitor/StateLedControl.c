/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时
 * StateLedControl.c
 *******************************************************************/
#include "StateLedControl.h"
#include "Board.h"
#include "Message.h"
#include "Console.h"
#include "stm32f1xx_hal.h"
#include "Gpio.h"
/********************************************************************
 *
 *******************************************************************/

#define STATE_TIMER_TIM_PRESCALE    (1296-1) 
#define STATE_TIMER_TIM_PERIOD_BASE	(512) 
#define TIMER1_CLK_ENABLE()          __HAL_RCC_TIM1_CLK_ENABLE()
#define STATE_TIMER_IRQHANDLE        TIM1_UP_IRQn

STATIC UINT16 timerTimes = 0;
STATIC TIM_HandleTypeDef timerDef;
/********************************************************************
 *
 *******************************************************************/
VOID StateLedTimerStartMs(UINT16 ms)
{
	ms = ms > 1200 ? 1200 : ms;
	UINT32 period = ms*STATE_TIMER_TIM_PERIOD_BASE/10;

	TIMER1_CLK_ENABLE();
	timerDef.Instance         = TIM1;
	timerDef.Init.Prescaler   = STATE_TIMER_TIM_PRESCALE;
	timerDef.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef.Init.Period      = period;
	HAL_TIM_Base_Init (&timerDef);

	HAL_NVIC_SetPriority (STATE_TIMER_IRQHANDLE, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (STATE_TIMER_IRQHANDLE);
	__HAL_TIM_CLEAR_IT(&timerDef, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT (&timerDef);
}

/********************************************************************
 *
 *******************************************************************/
VOID StateLedTimerStart(UINT16 ms, UINT16 times)
{
	timerTimes = times > 0 ? times-1: 0 ;
	StateLedTimerStartMs(ms);
}
/********************************************************************
 *
 *******************************************************************/
VOID StateLedTimerStop(VOID)
{
	StateLedTimerStartMs(1);
    HAL_TIM_Base_Stop_IT (&timerDef);
	timerTimes = 0;
}

/********************************************************************
 *
 *******************************************************************/
VOID TIM1_UP_IRQHandler(VOID)
{
	TIM_HandleTypeDef *htim = &timerDef;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_IRQHandler (&timerDef);
	STATIC UINT16 tLed=0;
	if (flag) {
		if(!timerTimes){
			GpioSet(BoardGetGpio(IO_LED1),tLed);
			if(tLed>0)
				tLed = 0;
			else
				tLed = 1;
		}else{
			timerTimes--;
		}
	}
}

/********************************************************************
 * END OF FILE
 *******************************************************************/

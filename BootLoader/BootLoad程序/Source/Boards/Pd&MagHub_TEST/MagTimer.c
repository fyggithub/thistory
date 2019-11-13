/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时1 用的是定时器7，定时2用的是定时器6
 * Timer.c
 *******************************************************************/

#include "MagTimer.h"
#include "Board.h"
#include "Message.h"
#include "Console.h"
#include "GlobalVariableSet.h"
#include "stm32f1xx_hal.h"
/********************************************************************
 *
 *******************************************************************/

// system clock is 66355200 Hz
#define TIMER_PRESCALER		(3600) // 72MHZ

STATIC TIM_HandleTypeDef timerDef4;

/********************************************************************
 *
 *******************************************************************/

VOID MagTimerInit(VOID)
{

	__HAL_RCC_TIM4_CLK_ENABLE();
	timerDef4.Instance         = TIM4;
	timerDef4.Init.Prescaler   = TIMER_PRESCALER;
	timerDef4.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef4.Init.Period      = 100;
	HAL_TIM_Base_Init (&timerDef4);

	HAL_NVIC_SetPriority (TIM4_IRQn, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (TIM4_IRQn);
	__HAL_TIM_CLEAR_IT(&timerDef4, TIM_IT_UPDATE);
	//HAL_TIM_Base_Start_IT (&timerDef4);
}

/********************************************************************
 *
 *******************************************************************/
VOID MagTimerStart(VOID)
{
    timerDef4.Instance         = TIM4;
    HAL_TIM_Base_Stop_IT (&timerDef4);
	timerDef4.Init.Prescaler   = TIMER_PRESCALER;
	timerDef4.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef4.Init.Period      = 100;
	HAL_TIM_Base_Init (&timerDef4);
    HAL_NVIC_SetPriority (TIM4_IRQn, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (TIM4_IRQn);
    __HAL_TIM_CLEAR_IT(&timerDef4, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT (&timerDef4);
}
/********************************************************************
 *
 *******************************************************************/
VOID TIM4_IRQHandler(VOID)
{
	TIM_HandleTypeDef *htim = &timerDef4;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	
	if (flag) {
        HAL_TIM_Base_Stop_IT (&timerDef4);
        if(GloVarGet()&& 0x10 == 0)
            return;
        Message msg;
        msg.id = MSG_MAG_TIMER;
        MessagePost(&msg);
	}
    HAL_TIM_IRQHandler (&timerDef4);
}


/********************************************************************
 * END OF FILE
 *******************************************************************/


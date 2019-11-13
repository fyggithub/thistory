/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时
 * PdTimer.c
 *******************************************************************/
#include "PdTimer.h"
#include "Board.h"
#include "Message.h"
#include "Console.h"
#include "GlobalVariableSet.h"
#include "stm32f1xx_hal.h"
#include "led.h"
#include "PdState.h"
/********************************************************************
 *
 *******************************************************************/
#define TIMER_PRESCALER		(3600) // 72MHZ
STATIC TIM_HandleTypeDef pdTimerDef;
STATIC UINT32 statePriod[4] = {0,15,25,60};
/********************************************************************
 *
 *******************************************************************/
VOID PdTimerStart(UINT32 period)
{
    //period = 100;
    pdTimerDef.Instance         = TIM1;
    HAL_TIM_Base_Stop_IT (&pdTimerDef);
    __HAL_RCC_TIM1_CLK_ENABLE();
    
	pdTimerDef.Init.Prescaler   = TIMER_PRESCALER;
	pdTimerDef.Init.CounterMode = TIM_COUNTERMODE_UP;
    pdTimerDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pdTimerDef.Init.Period      = period*10;
    pdTimerDef.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&pdTimerDef);

    HAL_NVIC_SetPriority (TIM1_UP_IRQn, TIMER_NVIC_PRIOR, 0);
   // pdTimerDef.Init.RepetitionCounter = 0;
    	HAL_NVIC_EnableIRQ   (TIM1_UP_IRQn);
	__HAL_TIM_CLEAR_IT(&pdTimerDef, TIM_IT_UPDATE);
    
    HAL_TIM_Base_Start_IT (&pdTimerDef);
}

/********************************************************************
 *
 *******************************************************************/
UINT8 PdTimerAskTime(UINT8 i)
{
    UINT8 x = statePriod[i] + pdTimerDef.Instance->CNT;
    return x;
}
/********************************************************************
 *
 *******************************************************************/
VOID TIM1_UP_IRQHandler(VOID)
{
	TIM_HandleTypeDef *htim = &pdTimerDef;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_IRQHandler (&pdTimerDef);   
	if (flag) {
        HAL_TIM_Base_Stop_IT (&pdTimerDef);
        if(GloVarGet()&& 0x0F == 0)
            return;
        Message msg;
        msg.id = MSG_PD_TIMER;
        MessagePost(&msg);           
	}
}

/********************************************************************
 * END OF FILE
 *******************************************************************/

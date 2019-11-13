/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时1 用的是定时器7，定时2用的是定时器6
 * Timer.c
 *******************************************************************/

#include "LiquidPwm.h"
/********************************************************************
 *
 *******************************************************************/

// system clock is 66355200 Hz
#define TIMER_PRESCALER		(67-1) // 

STATIC TIM_HandleTypeDef timerDef4;
volatile UINT16 timerTimes4 = 0;
STATIC UINT16 dutyNumber  = 0;
/********************************************************************
 *
 *******************************************************************/

VOID PwmInit(VOID)
{

	__HAL_RCC_TIM4_CLK_ENABLE();
	timerDef4.Instance         = TIM4;
	timerDef4.Init.Prescaler   = TIMER_PRESCALER;
	timerDef4.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef4.Init.Period      = 1;
	HAL_TIM_Base_Init (&timerDef4);

	HAL_NVIC_SetPriority (TIM4_IRQn, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (TIM4_IRQn);
	__HAL_TIM_CLEAR_IT(&timerDef4, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT (&timerDef4);
}

/********************************************************************
 *
 *******************************************************************/
//VOID Timer4Stop(VOID)
//{
//	PwmInit();
//    HAL_TIM_Base_Stop_IT (&timerDef4);
//}

/********************************************************************
 *
 *******************************************************************/
VOID TIM4_IRQHandler(VOID)
{
	TIM_HandleTypeDef *htim = &timerDef4;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	
	if (flag) {

		if(timerTimes4 < dutyNumber){
            HAL_GPIO_WritePin (GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
        }
        if(timerTimes4 > dutyNumber){
            HAL_GPIO_WritePin (GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
        }
        timerTimes4++;
        if(timerTimes4 >= 1000)
            timerTimes4 = 1;
	}
    HAL_TIM_IRQHandler (&timerDef4);
}

VOID PwmSetDutyRatio(DOUBLE ratio)
{
    DOUBLE temp = ratio * 1000;
    if(temp >= 1000)
        temp = 1000;
    else if(temp < 0)
        temp = 0;
    dutyNumber = (UINT16)temp;
}
/********************************************************************
 * END OF FILE
 *******************************************************************/


/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * TimerPd.c
 *******************************************************************/
#include "TimerPd.h"
#include "Board.h"
//#include "Event.h"
#include "Message.h"
#include "Datatypes.h"
#include "Bits.h"

/********************************************************************
 *
 *******************************************************************/
// system clock is 24000000 Hz
#define TIMER_PRESCALER1		(240) // 51.2 clk/ms
TIM_HandleTypeDef timerDef;
struct _tim3mod{
    UINT16 cnt;
    UINT32 time;
}tim3mod;
/********************************************************************
 *
 *******************************************************************/
VOID TimPdInit(VOID)
{
	UINT32 period = 99;
	TIMER3_CLK_ENABLE ();
	timerDef.Instance         	= TIM3;
	timerDef.Init.Prescaler   	= TIMER_PRESCALER1;
	timerDef.Init.CounterMode 	= TIM_COUNTERMODE_UP;
	timerDef.Init.Period      	= period;
	timerDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init (&timerDef);

	__HAL_TIM_ENABLE_IT	(&timerDef, TIM_IT_UPDATE);
	HAL_NVIC_SetPriority(TIM3_IRQn, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ	(TIM3_IRQn);
	__HAL_TIM_CLEAR_IT	(&timerDef, TIM_IT_UPDATE);
}

/********************************************************************
 *
 *******************************************************************/
UINT16 getTIM3cnt(VOID)
{
	tim3mod.cnt=timerDef.Instance->CNT;
	return tim3mod.cnt;
}
/********************************************************************
 *
 *******************************************************************/
UINT32 getTIM3time(VOID)
{
	return tim3mod.time;
}
/********************************************************************
 *
 *******************************************************************/
VOID setTIM3time(VOID)
{
	tim3mod.time=0;
}
/********************************************************************
 *
 *******************************************************************/
VOID tim3start(VOID)
{
	HAL_TIM_Base_Start(&timerDef);
}
/********************************************************************
 *
 *******************************************************************/
VOID TIM3_IRQHandler(VOID)
{
	Message msg;
	TIM_HandleTypeDef *htim = &timerDef;
		if(__HAL_TIM_GET_FLAG(&timerDef, TIM_FLAG_UPDATE) != RESET){
			if(tim3mod.cnt==0){
				msg.id=MSG_PD_0MS;
				MessagePost(&msg);
			}
			if(tim3mod.cnt==80){
				msg.id=MSG_PD_80MS;
				MessagePost(&msg);				
			}       
				tim3mod.cnt++;
			if(tim3mod.cnt==100){
				tim3mod.time++;
				tim3mod.cnt=0;
			} 
		}
	HAL_TIM_IRQHandler (htim);
}
/********************************************************************
 * END OF FILE
 *******************************************************************/


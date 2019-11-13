/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * Attention: 定义的定时1 用的是定时器7，定时2用的是定时器6
 * Tim4.c
 *******************************************************************/
#include "Tim4.h"
#include "Board.h"
#include "Message.h"
#include "Console.h"
#include "stm32f1xx_hal.h"
#include "Adc.h"
#include "Liquid.h"
#include "Led.h"
#include "math.h"
#include "Led.h"         
#define CALVIN_PARAMETER         273.15
#define VALTAGE_GAIN             3.12766
#define RES_12K                  12
/********************************************************************
*
*******************************************************************/
#define MIDDLE_d(a,b,c,d) \
	do{\
		if(a > b){\
		if(c > a) d = a;\
		else if(c < b) d = b;\
		else d = c;\
		}\
		else{\
			if(c < a) d = a;\
			else if(c > b) d = b;\
			else d = c;\
		}\
	}while(0)
// system clock is 66355200 Hz
#define TIMER_PRESCALER		(665-1) // 10us/clk
#define THRESHOLD 0.6
#define	THRESHOLDTIMES 10
UINT16 thresholdUint16;
STATIC TIM_HandleTypeDef timerDef;
UINT16 liquidSumValue;
UINT16 liquidMeanValue;
UINT16 liquidSumTimer;
UINT16 adjSumValue;
UINT16 adjMeanValue;
UINT16 ntcSumValue;
UINT16 ntcMeanValue;
UINT16 greaterThanThresholdTimes;
STATIC TIM_HandleTypeDef timerDef1;

/********************************************************************
 *
 *******************************************************************/
VOID AdcValueInit(VOID)
{
	thresholdUint16 = 0;
	liquidSumTimer = 0;
	adjSumValue = 0;
	adjMeanValue = 0;
    liquidSumValue = 0;
	liquidMeanValue = 0;
    ntcSumValue = 0;
    ntcMeanValue = 0;
	thresholdUint16 = THRESHOLD *4096 /3.3;
}
/********************************************************************
 *
 *******************************************************************/	
VOID Timer4Start(VOID)
{
	UINT32 period = 10;
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
	__HAL_RCC_TIM1_CLK_ENABLE();
	timerDef1.Instance         = TIM1;
	timerDef1.Init.Prescaler   = TIMER_PRESCALER;
	timerDef1.Init.CounterMode = TIM_COUNTERMODE_UP;
	timerDef1.Init.Period      = period;
    timerDef1.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&timerDef1);
    
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timerDef1, &sClockSourceConfig);
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timerDef1, &sMasterConfig);
    
	HAL_NVIC_SetPriority (TIM1_UP_IRQn, TIMER_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ   (TIM1_UP_IRQn);
    
	__HAL_TIM_CLEAR_IT(&timerDef1, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT (&timerDef1);
}

/********************************************************************
 *
 *******************************************************************/

VOID TIM1_UP_IRQHandler(VOID)
{
	
    TIM_HandleTypeDef *htim = &timerDef1;
	BOOL flag = __HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE);
	if (flag) {
        ntcSumValue +=  AdcValue[0];
        adjSumValue += AdcValue[1];
		liquidSumValue += AdcValue[2];
				
		liquidSumTimer++;
		if(liquidSumTimer >= 10){
            liquidSumTimer = 0;
            
            ntcMeanValue = ntcSumValue/10;
            adjMeanValue = adjSumValue/10;
            liquidMeanValue  = liquidSumValue/10;
            
            if(liquidMeanValue > thresholdUint16)
                greaterThanThresholdTimes++;
            else
                greaterThanThresholdTimes = 0;
            ntcSumValue = 0;
            adjSumValue = 0;
            liquidSumValue = 0;
		} 
	}
    HAL_TIM_IRQHandler (&timerDef1);
    HAL_ADCEx_InjectedStart_IT(&hadcx);
}
/********************************************************************
 *
 *******************************************************************/
BOOL IsLiquidContact(VOID)
{
    if(greaterThanThresholdTimes > THRESHOLDTIMES){
      TimerTest();
      return TRUE;
    }
    else
        return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
UINT16 ReadNtcValueUint16(VOID)
{
    return (ntcMeanValue & 0x0FFF);
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE ReadNtcValue(VOID)
{
    DOUBLE temp = (DOUBLE)(ntcMeanValue & 0x0FFF)/4096*3.3;
    DOUBLE r = RES_12K - temp * 10 / VALTAGE_GAIN;
    DOUBLE x = log(r);
    //by requirement document,the routine has simplied the expression
    DOUBLE y = 0.002666+0.000290 * x;//
	DOUBLE result = 1.0 / y - 273.15; 
    return result;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE ReadNtcVol(VOID)
{
    DOUBLE temp = (ntcMeanValue & 0x0FFF)*3.3/4096;
    return temp;
}
/********************************************************************
 *
 *******************************************************************/

DOUBLE ReadLqAdjValue(VOID)
{
    DOUBLE temp = ((DOUBLE)(adjMeanValue & 0x0FFF)/4096)*3.3;
    return temp;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE ReadLqValue(VOID)
{
    DOUBLE temp = liquidMeanValue & 0x0FFF;
    return temp*3.3/4096;
}
/********************************************************************
 * END OF FILE
 *******************************************************************/


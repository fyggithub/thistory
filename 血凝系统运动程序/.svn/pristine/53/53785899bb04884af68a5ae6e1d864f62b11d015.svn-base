/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Pwm.c
 *******************************************************************/
#include "Pwm.h"
#include "stm32f1xx_hal.h"
#include "Board.h"

/********************************************************************
 *
 *******************************************************************/
typedef struct _PwmInst{
	TIM_HandleTypeDef pwmHandle;
	UINT16 dutyRatio;
}PwmInst;

STATIC PwmInst pwmInst;
/********************************************************************
 *
 *******************************************************************/
VOID PwmInit(VOID)
{
	GPIO_InitTypeDef init;
	
	PWM_IOCLK_ENABLE ();
	init.Pin   = PWM_PIN;
	init.Mode  = GPIO_MODE_AF_PP;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PWM_IO, &init);

	TIM_ClockConfigTypeDef clock;
	PWM_CLK_ENABLE ();

	pwmInst.pwmHandle.Instance = PWM_TIM;
	pwmInst.pwmHandle.Init.Prescaler   = PWM_PRESCALER;
	pwmInst.pwmHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwmInst.pwmHandle.Init.Period      = PWM_PERIOD;
	pwmInst.pwmHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init (&(pwmInst.pwmHandle));

	clock.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource (&(pwmInst.pwmHandle), &clock);

	PwmSetDutyRatio(0);
	
	HAL_NVIC_SetPriority (PWM_IRQN, PWM_NVIC_PRIOR, 0);
	HAL_NVIC_EnableIRQ (PWM_IRQN);
}
/********************************************************************
 *
 *******************************************************************/
VOID PwmStart(VOID)
{
#ifdef STM32F051x8	
	HAL_TIM_PWM_Start_IT(&(pwmInst.pwmHandle), PWM_CHANNEL);
#else
	HAL_TIM_PWM_Start(&(pwmInst.pwmHandle), PWM_CHANNEL);
#endif
}
/********************************************************************
 *
 *******************************************************************/
VOID PwmStop(VOID)
{
#ifdef STM32F051x8	
	HAL_TIM_PWM_Stop_IT(&(pwmInst.pwmHandle), PWM_CHANNEL);
#else
	HAL_TIM_PWM_Stop(&(pwmInst.pwmHandle), PWM_CHANNEL);;
#endif
}
/********************************************************************
 *
 *******************************************************************/

VOID PwmSetDutyRatio(UINT32 ratioX1000)
{
	pwmInst.dutyRatio = ratioX1000;

	TIM_OC_InitTypeDef oc;
	oc.OCMode     = TIM_OCMODE_PWM2;
	oc.Pulse      = ratioX1000;
	oc.OCPolarity = TIM_OCPOLARITY;
	oc.OCFastMode = TIM_OCFAST_DISABLE;
#ifdef STM32F051x8	
#else
	oc.OCIdleState = TIM_OCIDLESTATE_RESET;      
	oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
#endif
	HAL_TIM_PWM_ConfigChannel (&(pwmInst.pwmHandle), &oc, PWM_CHANNEL);
}
/********************************************************************
 *
 *******************************************************************/

VOID PWM_IRQHANDLER(VOID)
{
	HAL_TIM_IRQHandler (&(pwmInst.pwmHandle));
}

/********************************************************************
 *
 *******************************************************************/



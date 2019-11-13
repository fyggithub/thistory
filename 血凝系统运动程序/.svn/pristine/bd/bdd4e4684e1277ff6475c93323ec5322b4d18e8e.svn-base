/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Pulse4Axis.c
 *******************************************************************/
#include "Pulse.h"
#include "Board.h"
/********************************************************************
 *
 *******************************************************************/
#define PULSE01_IO		    GPIOA
#define PULSE01_PIN		    GPIO_PIN_1
#define PULSE01_TIMER		TIM2
//#define PULSE01_CHANNEL	    TIM_CHANNEL_2
#define PULSE01_IO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define PULSE01_CLK_ENABLE()	__HAL_RCC_TIM2_CLK_ENABLE()
#define PULSE01_IRQHANDLER 	TIM2_IRQHandler
#define PULSE01_IRQ		    TIM2_IRQn

#define PULSE02_IO		    GPIOA
#define PULSE02_PIN		    GPIO_PIN_2
#define PULSE02_TIMER		TIM5
//#define PULSE02_CHANNEL	    TIM_CHANNEL_3
#define PULSE02_IO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define PULSE02_CLK_ENABLE()	__HAL_RCC_TIM5_CLK_ENABLE()
#define PULSE02_IRQHANDLER	TIM5_IRQHandler
#define PULSE02_IRQ		    TIM5_IRQn

#define PULSE03_IO		    GPIOB
#define PULSE03_PIN			GPIO_PIN_0
#define PULSE03_TIMER		TIM3
//#define PULSE03_CHANNEL		TIM_CHANNEL_3
#define PULSE03_IO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define PULSE03_CLK_ENABLE()	__HAL_RCC_TIM3_CLK_ENABLE()
#define PULSE03_IRQHANDLER	TIM3_IRQHandler
#define PULSE03_IRQ			TIM3_IRQn

#define PULSE04_IO			GPIOB
#define PULSE04_PIN			GPIO_PIN_1
#define PULSE04_TIMER		TIM4
//#define PULSE04_CHANNEL		TIM_CHANNEL_3
#define PULSE04_IO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define PULSE04_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define PULSE04_IRQHANDLER	TIM4_IRQHandler
#define PULSE04_IRQ			TIM4_IRQn

#define PULSE_DEF_PRESCALER	(96-1)
#define PULSE_DEF_PERIOD	(50)

#define PULSE_INIT_GPIO(PULSEID)\
	do{\
		GPIO_InitTypeDef init;\
		PULSEID##_IO_CLK_ENABLE ();\
		init.Pin   = PULSEID##_PIN;\
		init.Mode  = GPIO_MODE_OUTPUT_PP;\
		init.Speed = GPIO_SPEED_FREQ_HIGH;\
		HAL_GPIO_Init (PULSEID##_IO, &init);\
		HAL_GPIO_WritePin (PULSEID##_IO, PULSEID##_PIN, GPIO_PIN_RESET);\
	}while(0)

#define PULSE_INIT(PULSEID, handler)\
	do{\
		PULSEID##_CLK_ENABLE ();\
		handler.Instance         = PULSEID##_TIMER;\
		handler.Init.Prescaler   = PULSE_DEF_PRESCALER;\
		handler.Init.CounterMode = TIM_COUNTERMODE_UP;\
		handler.Init.Period      = PULSE_DEF_PERIOD;\
		HAL_TIM_Base_Init (&handler);\
	}while(0)

#define PULSE_INIT_IRQ(PULSEID, handler)\
	do{\
		HAL_NVIC_SetPriority (PULSEID##_IRQ, PULSE_NVIC_PRIOR, 0);\
		HAL_NVIC_EnableIRQ (PULSEID##_IRQ);\
		__HAL_TIM_CLEAR_IT(&handler, TIM_IT_UPDATE);\
	}while(0)
/********************************************************************
 *
 *******************************************************************/
STATIC Pulse pulses[PULSE_MAX];
/********************************************************************
 *
 *******************************************************************/
VOID PulseInit (Pulse* pulse, PULSE_CB callback)
{
	if (pulse) {
		switch(pulse->id){
		case PULSE01:
			pulse->io  = PULSE01_IO;
			pulse->pin = PULSE01_PIN;
			PULSE_INIT (PULSE01, (pulse->timHandler));
			PULSE_INIT_GPIO (PULSE01);
			PULSE_INIT_IRQ (PULSE01, (pulse->timHandler));
			break;
		case PULSE02:
			pulse->io  = PULSE02_IO;
			pulse->pin = PULSE02_PIN;
			PULSE_INIT (PULSE02, (pulse->timHandler));
			PULSE_INIT_GPIO (PULSE02);
			PULSE_INIT_IRQ (PULSE02, (pulse->timHandler));
			break;
		case PULSE03:
			pulse->io  = PULSE03_IO;
			pulse->pin = PULSE03_PIN;
			PULSE_INIT (PULSE03, (pulse->timHandler));
			PULSE_INIT_GPIO (PULSE03);
			PULSE_INIT_IRQ (PULSE03, (pulse->timHandler));
			break;
		case PULSE04:
			pulse->io  = PULSE04_IO;
			pulse->pin = PULSE04_PIN;
			PULSE_INIT (PULSE04, (pulse->timHandler));
			PULSE_INIT_GPIO (PULSE04);
			PULSE_INIT_IRQ (PULSE04, (pulse->timHandler));
			break;
		default:
			return;
		}
		HAL_GPIO_WritePin (pulse->io, pulse->pin, GPIO_PIN_RESET);
		pulse->callback = callback;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID PulseStart(Pulse* pulse)
{
	if(pulse) {
		TIM_HandleTypeDef* handler = &(pulse->timHandler);
		pulse->isEnable = TRUE;
		pulse->phase    = 0;
		HAL_GPIO_WritePin (pulse->io, pulse->pin, GPIO_PIN_RESET);
		__HAL_TIM_CLEAR_IT(handler, TIM_IT_UPDATE);
		HAL_TIM_Base_Start_IT (handler);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID PulseStop (Pulse* pulse)
{
	if(pulse){
		TIM_HandleTypeDef* handler = &(pulse->timHandler);
		HAL_TIM_Base_Stop (handler);
		pulse->isEnable = FALSE;
		pulse->phase    = 0;
		HAL_GPIO_WritePin (pulse->io, pulse->pin, GPIO_PIN_RESET);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID PulseSetFreq(Pulse* pulse, UINT32 freq)
{
	UINT32 sysCoreFreq = SystemCoreClock;
	UINT32 prescaler = (sysCoreFreq/100)/freq - 1;
	prescaler = prescaler >= 65535? 65535:prescaler;
	if(pulse){
		BOOL isEnable = pulse->isEnable;
		if(isEnable) PulseStop(pulse);
		pulse->timHandler.Init.Prescaler = prescaler;
		HAL_TIM_Base_Init (&(pulse->timHandler));
		if(isEnable) PulseStart(pulse);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID PulseSetOwner(Pulse* pulse, VOID* owner)
{
	if(pulse) pulse->owner = owner;
}
/********************************************************************
 *
 *******************************************************************/
Pulse* PulseGetObject(UINT16 id)
{
	if(id < PULSE_MAX){
		pulses[id].id = id;
		pulses[id].callback = NULL;
		pulses[id].io 		= NULL;
		pulses[id].pin 		= 0;
		pulses[id].owner	= NULL;
		return &pulses[id];
	}
	return NULL;
}
/********************************************************************
 *
 *******************************************************************/
VOID HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Pulse* pulse = NULL;
	if(htim == (&(pulses[PULSE01].timHandler))) pulse = &pulses[PULSE01];
	if(htim == (&(pulses[PULSE02].timHandler))) pulse = &pulses[PULSE02];
	if(htim == (&(pulses[PULSE03].timHandler))) pulse = &pulses[PULSE03];
	if(htim == (&(pulses[PULSE04].timHandler))) pulse = &pulses[PULSE04];
	if(pulse && pulse->isEnable) {
		pulse->phase = 1 - pulse->phase;
		if(pulse->phase == 1){
			HAL_GPIO_WritePin (pulse->io, pulse->pin, GPIO_PIN_SET);
		}
		if(pulse->phase == 0){
			HAL_GPIO_WritePin (pulse->io, pulse->pin, GPIO_PIN_RESET);
			if(pulse->callback) pulse->callback(pulse);
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
#ifdef EN_PULSE01_IRQ
	VOID PULSE01_IRQHANDLER(VOID)
	{
		HAL_TIM_IRQHandler (&(pulses[PULSE01].timHandler));
	}
#endif
#ifdef EN_PULSE02_IRQ
	VOID PULSE02_IRQHANDLER(VOID)
	{
		HAL_TIM_IRQHandler (&(pulses[PULSE02].timHandler));
	}
#endif
#ifdef EN_PULSE03_IRQ
	VOID PULSE03_IRQHANDLER(VOID)
	{
		HAL_TIM_IRQHandler (&(pulses[PULSE03].timHandler));
	}
#endif
#ifdef EN_PULSE04_IRQ
	VOID PULSE04_IRQHANDLER(VOID)
	{
		HAL_TIM_IRQHandler (&(pulses[PULSE04].timHandler));
	}
#endif
/********************************************************************
 * End of File
 *******************************************************************/


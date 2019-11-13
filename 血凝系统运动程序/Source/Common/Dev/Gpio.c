/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Gpio.c
 *******************************************************************/
#include "Gpio.h"
/********************************************************************
 *
 *******************************************************************/
STATIC VOID GpioClkEnable(GPIO_TypeDef* gpioTypeDef)
{
	if(gpioTypeDef == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	} else if (gpioTypeDef == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
#ifndef STM32F051x8
	} else if (gpioTypeDef == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	} else if (gpioTypeDef == GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
	} else if (gpioTypeDef == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOF
	} else if (gpioTypeDef == GPIOF) {
		__HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#ifdef GPIOG
	} else if (gpioTypeDef == GPIOG) {
		__HAL_RCC_GPIOG_CLK_ENABLE();
#endif
	} else {
		; // nothing to do
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID   GpioInit(Gpio* gpio, UINT32 mode, UINT32 speed)
{
	GPIO_InitTypeDef init;
	if ( gpio && gpio->io){
		GpioClkEnable(gpio->io);
		HAL_GPIO_WritePin (gpio->io, gpio->pin, GPIO_PIN_RESET);
		init.Pin   = gpio->pin;
		init.Mode  = mode;
		init.Speed = speed;
		HAL_GPIO_Init(gpio->io, &init);
	}
}

/********************************************************************
 *
 *******************************************************************/
VOID   GpioHighInit(Gpio* gpio, UINT32 mode, UINT32 speed)
{
	GPIO_InitTypeDef init;
	if ( gpio && gpio->io){
		GpioClkEnable(gpio->io);
		HAL_GPIO_WritePin (gpio->io, gpio->pin, GPIO_PIN_SET);
		init.Pin   = gpio->pin;
		init.Mode  = mode;
		init.Speed = speed;
		HAL_GPIO_Init(gpio->io, &init);
	}
}

VOID GpioExitInit(Gpio* gpio, UINT32 mode, UINT32 pull, IRQn_Type IRQn)
{
	GPIO_InitTypeDef init;
	if ( gpio && gpio->io){
		GpioClkEnable(gpio->io);
		init.Pin   = gpio->pin;
		init.Mode  = mode;
		init.Pull =  pull;
		HAL_GPIO_Init(gpio->io, &init);

		HAL_NVIC_SetPriority(IRQn, 2, 1);
		HAL_NVIC_EnableIRQ(IRQn);
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT16 GpioGet (Gpio* gpio)
{
	if (gpio && gpio->io) {
		return (UINT16) HAL_GPIO_ReadPin (gpio->io, gpio->pin);
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
VOID   GpioSet (Gpio* gpio, UINT16 v)
{
	if (gpio && gpio->io) {
		if (v) {
			HAL_GPIO_WritePin (gpio->io, gpio->pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin (gpio->io, gpio->pin, GPIO_PIN_RESET);
		}
	}
}
/********************************************************************
 * END OF FILE
 *******************************************************************/


/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Gpio.h
 *******************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_
/********************************************************************
 * Gpio struct
 *******************************************************************/
#include "Datatypes.h"
#ifdef STM32F051x8
#include "stm32f0xx_hal.h"
#else
#include "stm32f1xx_hal.h"
#endif

typedef struct __Gpio{
	GPIO_TypeDef* io;
	UINT16		  pin;
}Gpio;
/********************************************************************
 * Gpio Method
 *******************************************************************/
VOID   GpioInit(Gpio* gpio, UINT32 mode, UINT32 speed);
VOID   GpioHighInit(Gpio* gpio, UINT32 mode, UINT32 speed);
VOID   GpioExitInit(Gpio* gpio, UINT32 mode, UINT32 pull, IRQn_Type IRQn);
UINT16 GpioGet (Gpio* gpio);
VOID   GpioSet (Gpio* gpio, UINT16 v);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _GPIO_H_

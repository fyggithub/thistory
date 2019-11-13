/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Board.h
 *******************************************************************/
#ifndef _BOARD_H_
#define _BOARD_H_
/********************************************************************
 * Macro defined
 *******************************************************************/
#include "Datatypes.h"
#include "Uart.h"
#include "Gpio.h"
#include "UartF103CB.h"
/********************************************************************
 *
 *******************************************************************/
#define PWM_NVIC_PRIOR   (1)
#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
	MSG_HOST_VERSION = 20,
	MSG_TIMER,
	MSG_TIMER2,
	MSG_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define EN_CONSOLE
#define EN_UART01
#define EN_UART02

enum{
	IO_LED1 = 0, IO_LED2, IO_FAN1, IO_FAN2, IO_FAN3,
	IO_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define PWM_IO		GPIOB
#define PWM_PIN		GPIO_PIN_6
#define PWM_TIM		TIM4
#define PWM_CHANNEL TIM_CHANNEL_1
#define PWM_IRQN	TIM4_IRQn

#define PWM_IOCLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define TIM_OCPOLARITY TIM_OCPOLARITY_LOW
#define PWM_PRESCALER	(768-1)
#define PWM_PERIOD		(1000)
/********************************************************************
 *
 *******************************************************************/
#define _COOL_PID
//#define KIOLIMIT 0.6

/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetConsoleUart(VOID);
Uart* BoardGetHostUart(VOID);
Gpio* BoardGetGpio(UINT16 id);

VOID  BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

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
#include "Spi.h"

/********************************************************************
 *
 *******************************************************************/
#define _COOL_PID
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
	IO_LED1 = 0, IO_LED2,
	IO_MAX,
};


/********************************************************************
 *TIM4's PWM
*******************************************************************/
#define PWM_IO		GPIOB
#define PWM_PIN		GPIO_PIN_9
#define PWM_TIM		TIM4
#define PWM_CHANNEL TIM_CHANNEL_4
#define PWM_IRQN	TIM4_IRQn

#define PWM_IOCLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()

#define PWM_PRESCALER	(65)
#define PWM_PERIOD		(1000)
#define TIM_OCPOLARITY  TIM_OCPOLARITY_HIGH
/********************************************************************
 *SPI
*******************************************************************/
#define SPI_DATASIZE  SPI_DATASIZE_16BIT

/********************************************************************
 *
*******************************************************************/
Uart* BoardGetConsoleUart(VOID);
Uart* BoardGetHostUart(VOID);
Gpio* BoardGetGpio(UINT16 id);
Spi*  BoardGetSpiExt(VOID);
VOID  BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

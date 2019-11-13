/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * UartF051X8.h
 *******************************************************************/
#ifndef _UARTF051X8_H
#define _UARTF051X8_H
/********************************************************************
 *
 *******************************************************************/
#include "stm32f0xx_hal.h"
enum{
	UART01=0, UART02, UART_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define UART01_IO_TX	GPIOB
#define UART01_IO_RX	GPIOB
#define UART01_PIN_TX	GPIO_PIN_6
#define UART01_PIN_RX	GPIO_PIN_7
#define UART01_MODE_TX	GPIO_MODE_AF_PP
#define UART01_MODE_RX	GPIO_MODE_AF_PP
#define UART01_SPEED_TX	GPIO_SPEED_FREQ_HIGH
#define UART01_PULL_RX	GPIO_PULLUP
#define UART01_IO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define UART01_ALTERNATE    GPIO_AF0_USART1

#define UART01_HANDLE		USART1
#define UART01_BAUDRATE		(115200)
#define UART01_WORDLENGTH	UART_WORDLENGTH_8B
#define UART01_STOPBITS		UART_STOPBITS_1
#define UART01_PARITY		UART_PARITY_NONE
#define UART01_MODE 		UART_MODE_TX_RX
#define UART01_HWFLOWCTL	UART_HWCONTROL_NONE
#define UART01_OVERSAMPLING	UART_OVERSAMPLING_16
#define UART01_CLK_ENABLE()	__HAL_RCC_USART1_CLK_ENABLE()

#define UART01_IRQHANDLER	USART1_IRQHandler
#define UART01_IRQ			USART1_IRQn


#define UART02_IO_TX	GPIOA
#define UART02_IO_RX	GPIOA
#define UART02_PIN_TX	GPIO_PIN_2
#define UART02_PIN_RX	GPIO_PIN_3
#define UART02_MODE_TX	GPIO_MODE_AF_PP
#define UART02_MODE_RX	GPIO_MODE_AF_PP
#define UART02_SPEED_TX	GPIO_SPEED_FREQ_HIGH
#define UART02_PULL_RX	GPIO_PULLUP
#define UART02_IO_CLK_ENABLE() 	__HAL_RCC_GPIOA_CLK_ENABLE()
#define UART02_ALTERNATE    GPIO_AF1_USART2

#define UART02_HANDLE 		USART2
#define UART02_BAUDRATE		(115200)
#define UART02_WORDLENGTH	UART_WORDLENGTH_8B
#define UART02_STOPBITS		UART_STOPBITS_1
#define UART02_PARITY		UART_PARITY_NONE
#define UART02_MODE 		UART_MODE_TX_RX
#define UART02_HWFLOWCTL	UART_HWCONTROL_NONE
#define UART02_OVERSAMPLING	UART_OVERSAMPLING_16
#define UART02_CLK_ENABLE()	__HAL_RCC_USART2_CLK_ENABLE()

#define UART02_IRQHANDLER	USART2_IRQHandler
#define UART02_IRQ			USART2_IRQn
/********************************************************************
 *
 *******************************************************************/
#endif



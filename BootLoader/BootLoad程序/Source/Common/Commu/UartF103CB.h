/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * UartF103CB.h
 *******************************************************************/
#ifndef _UARTF103CB_H_
#define _UARTF103CB_H_
/********************************************************************
 *
 *******************************************************************/
enum{
	UART01=0, UART02, UART03, UART_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define UART01_IO_TX	        GPIOA
#define UART01_IO_RX	        GPIOA
#define UART01_PIN_TX	        GPIO_PIN_9
#define UART01_PIN_RX	        GPIO_PIN_10
#define UART01_MODE_TX	        GPIO_MODE_AF_PP
#define UART01_MODE_RX	        GPIO_MODE_INPUT
#define UART01_SPEED_TX	        GPIO_SPEED_FREQ_HIGH
#define UART01_PULL_RX	        GPIO_NOPULL
#define UART01_IO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()

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
/********************************************************************
 *
 *******************************************************************/
#define UART02_IO_TX	        GPIOA
#define UART02_IO_RX	        GPIOA
#define UART02_PIN_TX	        GPIO_PIN_2
#define UART02_PIN_RX	        GPIO_PIN_3
#define UART02_MODE_TX	        GPIO_MODE_AF_PP
#define UART02_MODE_RX	        GPIO_MODE_INPUT
#define UART02_SPEED_TX	        GPIO_SPEED_FREQ_HIGH
#define UART02_PULL_RX	        GPIO_NOPULL
#define UART02_IO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()

#define UART02_HANDLE		USART2
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

#define UART03_IO_TX	        GPIOB
#define UART03_IO_RX	        GPIOB
#define UART03_PIN_TX	        GPIO_PIN_10
#define UART03_PIN_RX	        GPIO_PIN_11
#define UART03_MODE_TX	        GPIO_MODE_AF_PP
#define UART03_MODE_RX	        GPIO_MODE_INPUT
#define UART03_SPEED_TX	        GPIO_SPEED_FREQ_HIGH
#define UART03_PULL_RX	        GPIO_NOPULL
#define UART03_IO_CLK_ENABLE() 	__HAL_RCC_GPIOB_CLK_ENABLE()

#define UART03_HANDLE 		(USART3)
#define UART03_BAUDRATE		(115200)
#define UART03_WORDLENGTH	UART_WORDLENGTH_8B
#define UART03_STOPBITS		UART_STOPBITS_1
#define UART03_PARITY		UART_PARITY_NONE
#define UART03_MODE 		UART_MODE_TX_RX
#define UART03_HWFLOWCTL	UART_HWCONTROL_NONE
#define UART03_OVERSAMPLING	UART_OVERSAMPLING_16
#define UART03_CLK_ENABLE()	__HAL_RCC_USART3_CLK_ENABLE()

#define UART03_IRQHANDLER	USART3_IRQHandler
#define UART03_IRQ			USART3_IRQn
/********************************************************************
 *
 *******************************************************************/
#endif // _UARTF103CB_H_

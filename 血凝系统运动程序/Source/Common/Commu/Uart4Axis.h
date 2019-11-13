/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Uart4Axis.c
 *******************************************************************/
#ifndef _UART4AXIS_H_
#define _UART4AXIS_H_
/********************************************************************
 *
 *******************************************************************/

enum{
	UART01=0, UART02, UART03, UART04, UART05, UART_MAX,
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

#define UART04_IO_TX	        GPIOC
#define UART04_IO_RX	        GPIOC
#define UART04_PIN_TX	        GPIO_PIN_10
#define UART04_PIN_RX	        GPIO_PIN_11
#define UART04_MODE_TX	        GPIO_MODE_AF_PP
#define UART04_MODE_RX	        GPIO_MODE_INPUT
#define UART04_SPEED_TX	        GPIO_SPEED_FREQ_HIGH
#define UART04_PULL_RX	        GPIO_NOPULL
#define UART04_IO_CLK_ENABLE() 	__HAL_RCC_GPIOC_CLK_ENABLE()

#define UART04_HANDLE 		UART4
#define UART04_BAUDRATE		(115200)
#define UART04_WORDLENGTH	UART_WORDLENGTH_8B
#define UART04_STOPBITS		UART_STOPBITS_1
#define UART04_PARITY		UART_PARITY_NONE
#define UART04_MODE 		UART_MODE_TX_RX
#define UART04_HWFLOWCTL	UART_HWCONTROL_NONE
#define UART04_OVERSAMPLING	UART_OVERSAMPLING_16
#define UART04_CLK_ENABLE()	__HAL_RCC_UART4_CLK_ENABLE()

#define UART04_IRQHANDLER	UART4_IRQHandler
#define UART04_IRQ			UART4_IRQn
/********************************************************************
 *
 *******************************************************************/

#define UART05_IO_TX	        GPIOC
#define UART05_IO_RX	        GPIOD
#define UART05_PIN_TX	        GPIO_PIN_12
#define UART05_PIN_RX	        GPIO_PIN_2
#define UART05_MODE_TX	        GPIO_MODE_AF_PP
#define UART05_MODE_RX	        GPIO_MODE_INPUT
#define UART05_SPEED_TX	        GPIO_SPEED_FREQ_HIGH
#define UART05_PULL_RX	        GPIO_NOPULL
#define UART05_IO_CLK_ENABLE() 	\
			 	do{\
					__HAL_RCC_GPIOC_CLK_ENABLE();\
					__HAL_RCC_GPIOD_CLK_ENABLE();\
				}while(0);

#define UART05_HANDLE 		(UART5)
#define UART05_BAUDRATE		(115200)
#define UART05_WORDLENGTH	UART_WORDLENGTH_8B
#define UART05_STOPBITS		UART_STOPBITS_1
#define UART05_PARITY		UART_PARITY_NONE
#define UART05_MODE 		UART_MODE_TX_RX
#define UART05_HWFLOWCTL	UART_HWCONTROL_NONE
#define UART05_OVERSAMPLING	UART_OVERSAMPLING_16
#define UART05_CLK_ENABLE()	__HAL_RCC_UART5_CLK_ENABLE()

#define UART05_IRQHANDLER	UART5_IRQHandler
#define UART05_IRQ			UART5_IRQn
/********************************************************************
 *
 *******************************************************************/
#endif // _UART4AXIS_H_

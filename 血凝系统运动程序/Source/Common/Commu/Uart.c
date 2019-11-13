/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Uart4Axis.c
 *******************************************************************/
#include "Board.h"
#include <string.h>
/********************************************************************
 *
 *******************************************************************/

#ifdef STM32F051x8
#define UART_ALTERNATE(UARTID) do{gpioInit.Alternate = UARTID##_ALTERNATE;}while(0)
#else
#define UART_ALTERNATE(UARTID)
#endif

/********************************************************************
 *
 *******************************************************************/

#define UART_INIT_GPIO(UARTID) \
	do{\
		GPIO_InitTypeDef gpioInit;\
		UARTID##_IO_CLK_ENABLE();\
		\
		gpioInit.Pin	   = UARTID##_PIN_TX;\
		gpioInit.Mode	   = UARTID##_MODE_TX;\
		gpioInit.Speed	   = UARTID##_SPEED_TX;\
        UART_ALTERNATE(UARTID);\
		HAL_GPIO_Init(UARTID##_IO_TX, &gpioInit);\
		\
		gpioInit.Pin 	= UARTID##_PIN_RX;\
		gpioInit.Mode 	= UARTID##_MODE_RX;\
		gpioInit.Pull	= UARTID##_PULL_RX;\
		HAL_GPIO_Init(UARTID##_IO_RX, &gpioInit);\
	}while(0)

/********************************************************************
 *
 *******************************************************************/

#define UART_INIT(UARTID, handle)\
	do{\
		UARTID##_CLK_ENABLE();\
		\
		handle.Instance = UARTID##_HANDLE;\
		handle.Init.BaudRate	= UARTID##_BAUDRATE;\
		handle.Init.WordLength	= UARTID##_WORDLENGTH;\
		handle.Init.StopBits	= UARTID##_STOPBITS;\
		handle.Init.Parity		= UARTID##_PARITY;\
		handle.Init.Mode		= UARTID##_MODE;\
		handle.Init.HwFlowCtl	= UARTID##_HWFLOWCTL;\
		handle.Init.OverSampling= UARTID##_OVERSAMPLING;\
		HAL_UART_Init(&handle);\
		\
		HAL_NVIC_SetPriority(UARTID##_IRQ, UART_NVIC_PRIOR, 0);\
		HAL_NVIC_EnableIRQ(UARTID##_IRQ);\
	}while(0)

/********************************************************************
 *
 *******************************************************************/

STATIC Uart uarts[UART_MAX];

/********************************************************************
 *
 *******************************************************************/
VOID   UartInit(Uart* uart, Buffer* rxBuf, BYTE* txArr, UINT16 txSize)
{
	if (uart && uart->id < UART_MAX) {
		uart->rxBuffer = rxBuf;
		uart->txArray  = txArr;
		uart->txSize   = txSize;
		switch(uart->id){
#ifdef EN_UART01
		case UART01:
			UART_INIT(UART01, (uart->uartDef));
			UART_INIT_GPIO(UART01);
			break;
#endif
#ifdef EN_UART02
		case UART02:
			UART_INIT(UART02, (uart->uartDef));
        	UART_INIT_GPIO(UART02);
        break;
#endif
#ifdef EN_UART03
		case UART03:
			UART_INIT(UART03, (uart->uartDef));
			UART_INIT_GPIO(UART03);
			break;
#endif
#ifdef EN_UART04
		case UART04:
			UART_INIT(UART04, (uart->uartDef));
			UART_INIT_GPIO(UART04);
			break;
#endif
#ifdef EN_UART05
		case UART05:
			UART_INIT(UART05, (uart->uartDef));
			UART_INIT_GPIO(UART05);
			break;
#endif
		default:
			return; // do nothing!!!
		}
		HAL_UART_Receive_IT ( &(uart->uartDef),
			&(uart->rxChar), 1);
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT16 UartSend(Uart* uart, BYTE* arr, UINT16 len)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	HAL_UART_StateTypeDef uartState;
	if (uart && arr && len>0) {
		UART_HandleTypeDef* handler = &(uart->uartDef);
		BYTE* txArr = uart->txArray;
		if (handler && txArr) {
			len = len > uart->txSize ? uart->txSize : len;
			// wait for uart tx ready.
			do{
				uartState = HAL_UART_GetState(handler);
			}while(uartState != HAL_UART_STATE_BUSY_RX &&
				uartState != HAL_UART_STATE_READY);
			// start sending
			memcpy(txArr, arr, len);
			status = HAL_UART_Transmit_IT(handler, txArr, len);
			if (status == HAL_OK)
				return len;
		}
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL isUartEnable(UINT16 id)
{
	if( id < UART_MAX){
		switch(id){
#ifdef EN_UART01
		case UART01:
			return TRUE;
#endif
#ifdef EN_UART02
		case UART02:
			return TRUE;
#endif
#ifdef EN_UART03
		case UART03:
			return TRUE;
#endif
#ifdef EN_UART04
		case UART04:
			return TRUE;
#endif
#ifdef EN_UART05
		case UART05:
			return TRUE;
#endif
		default:
			break;
		}
	}
	return FALSE;
}

/********************************************************************
 *
 *******************************************************************/

Uart*  UartGetObject(UINT16 id)
{
	if(isUartEnable (id)){
		uarts[id].id       = id;
		uarts[id].rxBuffer = NULL;
		uarts[id].txArray  = NULL;
		uarts[id].txSize   = 0;
		return &(uarts[id]);
	}
	return NULL;
}
/********************************************************************
 *
 *******************************************************************/
VOID HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUart)
{
	for (UINT16 i=0; i<UART_MAX; i++) {
		UART_HandleTypeDef* handler = &(uarts[i].uartDef);
		if ( handler == hUart ){
			if ( uarts[i].rxBuffer ){
				Buffer* buffer = uarts[i].rxBuffer;
				BYTE by        = uarts[i].rxChar;
				BufferAppendByte ( buffer, by );
				HAL_UART_Receive_IT (handler, &(uarts[i].rxChar), 1);
				break;
			}
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
#ifdef EN_UART01
	VOID UART01_IRQHANDLER(VOID)
	{
		HAL_UART_IRQHandler( &(uarts[UART01].uartDef) );
	}
#endif
#ifdef EN_UART02
	VOID UART02_IRQHANDLER(VOID)
	{
		HAL_UART_IRQHandler( &(uarts[UART02].uartDef) );
	}
#endif
#ifdef EN_UART03
	VOID UART03_IRQHANDLER(VOID)
	{
		HAL_UART_IRQHandler( &(uarts[UART03].uartDef) );
	}
	#endif
#ifdef EN_UART04
	VOID UART04_IRQHANDLER(VOID)
	{
		HAL_UART_IRQHandler( &(uarts[UART04].uartDef) );
	}
#endif
#ifdef EN_UART05
	VOID UART05_IRQHANDLER(VOID)
	{
		HAL_UART_IRQHandler( &(uarts[UART05].uartDef) );
	}
#endif

/********************************************************************
 * END OF FILE
 *******************************************************************/

/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Uart.h
 *******************************************************************/
#ifndef _UART_H_
#define _UART_H_
/********************************************************************
 * Uart struct
 *******************************************************************/
#include "Datatypes.h"
#include "Buffer.h"
#ifdef STM32F051x8
#include "stm32f0xx_hal.h"
#else
#include "stm32f1xx_hal.h"
#endif
/********************************************************************
 * Uart struct
 *******************************************************************/
typedef struct __Uart{
	UINT16    id;
	Buffer*   rxBuffer;
	BYTE*	  txArray;
	UINT16	  txSize;
	BYTE	  rxChar;
	UART_HandleTypeDef uartDef;
}Uart;
/********************************************************************
 * Uart Method
 *******************************************************************/
VOID   UartInit(Uart* uart, Buffer* rxBuf, BYTE* txArr, UINT16 txSize);
UINT16 UartSend(Uart* uart, BYTE* arr, UINT16 len);
Uart*  UartGetObject(UINT16 id);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _UART_H_

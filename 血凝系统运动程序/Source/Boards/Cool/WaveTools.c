/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * WaveTools.c
 *******************************************************************/
#include "Uart.h"
#include "Board.h"
#include "Buffer.h"
#include "WaveTools.h"
/********************************************************************
 *
 *******************************************************************/
#define BUF_LEN	(8)
typedef struct _WaveTools{
	Uart*  uart;
	BYTE   rxBufferArr[BUF_LEN];
	Buffer rxBuffer;
	BYTE   txArr[BUF_LEN];
}WaveTools;
STATIC WaveTools wave;
/********************************************************************
 *
 *******************************************************************/
VOID WaveInit(VOID)
{
	wave.uart = BoardGetHostUart ();
	BufferCreate (&(wave.rxBuffer), wave.rxBufferArr, BUF_LEN);
	UartInit(wave.uart, &(wave.rxBuffer), wave.txArr, BUF_LEN);
}
/********************************************************************
 *
 *******************************************************************/

VOID WaveData(UINT8 data)
{
	if(wave.uart){
		UartSend(wave.uart, (BYTE*)&data, 1);
	}
}
/********************************************************************
 *
 *******************************************************************/



/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Spi.h
 *******************************************************************/
#ifndef _SPI_H_
#define _SPI_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
#ifdef STM32F051x8
#include "stm32f0xx_hal.h"
#else
#include "stm32f1xx_hal.h"
#endif
#include "Gpio.h"

/********************************************************************
 *
 *******************************************************************/
enum{
	SPI01=0, SPI02, SPI_MAX,
};
typedef struct __Spi{
	UINT16 id;
	UINT32 clkPolarity;
	UINT32 clkPhase;
	UINT32 bdrPrescaler;
	UINT32 firstBit;
	UINT32 dataSize;
	Gpio   csPin;
	SPI_HandleTypeDef spiDef;
}Spi;
/********************************************************************
 *
 *******************************************************************/
VOID   SpiInit(Spi* spi);
UINT16 SpiSend(Spi* spi, BYTE* data, UINT16 len);
UINT16 SpiRecv(Spi* spi, BYTE* data, UINT16 len);
VOID   SpiEnableCS(Spi* spi);
VOID   SpiDisableCS(Spi* spi);
Spi*   SpiGetObject(UINT16 id);
UINT16 SpiSendRecv(Spi* spi, BYTE* pTxData,BYTE* pRxData, UINT16 len);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _SPI_H_



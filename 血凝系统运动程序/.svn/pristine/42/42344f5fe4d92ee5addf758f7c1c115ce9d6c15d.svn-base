/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * AD7928.c
 *******************************************************************/
#include "AD7928.h"
#include "Console.h"
#include "Spi.h"
#include "stm32f0xx_hal.h"
/********************************************************************
 *
 *******************************************************************/
#define CTRL_WRITE		(0x8000)
#define CTRL_SEQ		(0x4000)
#define CTRL_PM_NORM	(0x0300)
#define CTRL_SHADOW		(0x0080)
#define CTRL_CODING		(0x0010)
#define CTRL_ADDR_MAX	(0x0C00)

#define CTRL_CMD (CTRL_WRITE | CTRL_SEQ |\
				  CTRL_PM_NORM | CTRL_ADDR_MAX |\
			   	  CTRL_SHADOW | CTRL_CODING)
/********************************************************************
 *
 *******************************************************************/
STATIC Spi* spi = NULL;
DWORD adValue[AD_VINMAX];
/********************************************************************
 *
 *******************************************************************/
STATIC WORD AD7928Recv(VOID)
{
	WORD data = 0;
	if(spi){
		SpiEnableCS (spi);
		SpiRecv(spi, (BYTE*)&data, 1);
		SpiDisableCS (spi);
	}
	return data;
}
/********************************************************************
 *
 *******************************************************************/
VOID AD7928Update(VOID)
{
	WORD cmd = CTRL_CMD;
	SpiEnableCS (spi);
	SpiSend(spi, (BYTE*)&cmd, 1);
	SpiDisableCS (spi);	
	for(INT32 i=AD_VIN0; i<AD_VINMAX; i++){
		WORD data = AD7928Recv ();
		WORD addr = ((data&0x7000)>>12);
		if(addr < AD_VINMAX){
			WORD v = (data&0xfff);
			adValue[addr] = v;
		}
	}
}
/********************************************************************
 *
 *******************************************************************/

WORD AD7928Read(UINT16 channel)
{
	if(channel < AD_VINMAX){
		return adValue[channel];
	}
	return 0;
}

/********************************************************************
 *
 *******************************************************************/

VOID AD7928Init (VOID)
{
	spi = BoardGetAdSpi ();
	spi->csPin.io  = GPIOB;
	spi->csPin.pin = GPIO_PIN_0;

	spi->bdrPrescaler = SPI_BAUDRATEPRESCALER_8;
	spi->firstBit     = SPI_FIRSTBIT_MSB;
	spi->dataSize     = SPI_DATASIZE_16BIT;
	spi->clkPolarity  = SPI_POLARITY_HIGH;	// CPOL = 1
	spi->clkPhase     = SPI_PHASE_1EDGE;    // CPHA = 0

	SpiInit(spi);
}


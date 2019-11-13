/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * FramFM25CL64B.c
 *******************************************************************/
#include "Fram.h"
#include "Bits.h"
#include "Board.h"
#include "Spi.h"
#include "stm32f1xx_hal.h"
#include "Console.h"
/********************************************************************
 *
 *******************************************************************/
#define CMD_WRITE_ENABLE	(0x06)
#define CMD_WRITE_DISABLE	(0x04)
#define CMD_READ_STATUS		(0x05)
#define CMD_WRITE_STATUS	(0x01)
#define CMD_READ_DATA		(0x03)
#define CMD_WRITE_DATA		(0x02)

#define FRAM_ADDR_MAX	(0x1FFF)

#define STATUS_WPEN	(BIT(7))
#define STATUS_BP	(BIT(3)|BIT(2))
#define STATUS_WEL	(BIT(1))

STATIC Spi* spi = NULL;
/********************************************************************
 *
 *******************************************************************/
STATIC VOID FramWriteEnable(VOID)
{
	if(spi){
		BYTE cmd = CMD_WRITE_ENABLE;
		SpiEnableCS (spi);
		SpiSend(spi, &cmd, 1);
		SpiDisableCS (spi);
	}
}
STATIC VOID FramWriteDisable(VOID)
{
	if(spi){
		BYTE cmd = CMD_WRITE_DISABLE;
		SpiEnableCS (spi);
		SpiSend(spi, &cmd, 1);
		SpiDisableCS (spi);
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC BYTE FramReadStatus(VOID)
{
	if(spi){
		BYTE cmd = CMD_READ_STATUS;
		BYTE status = 0xFF;
		SpiEnableCS (spi);
		SpiSend (spi, &cmd, 1);
		SpiRecv (spi, &status, 1);
		SpiDisableCS (spi);
		return status;
	}
	return 0xFF;
}

/********************************************************************
 *
 *******************************************************************/
VOID FramRead (UINT16 addr, BYTE* data, UINT16 len)
{
	if (spi) {
		BYTE by;
		BYTE cmd[3] = {CMD_READ_DATA, 0, 0};
		UINT16 i = 0;
		SpiEnableCS (spi);
		cmd[1] = HBYTE (addr);
		cmd[2] = LBYTE (addr);
		SpiSend(spi, cmd, 3);
		while(addr <= FRAM_ADDR_MAX){
			if(i>=len) break;
			by = 0xCC;
			SpiRecv(spi, &by, 1);
			data[i++] = by;
			addr++;
		}
		SpiDisableCS (spi);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID FramWrite(UINT16 addr, BYTE* data, UINT16 len)
{
	if (spi) {
		BYTE by;
		BYTE cmd[3] = {CMD_WRITE_DATA, 0, 0};
		UINT16 i = 0;
		FramWriteEnable ();
		SpiEnableCS (spi);
		cmd[1] = HBYTE (addr);
		cmd[2] = LBYTE (addr);
		SpiSend(spi, cmd, 3);
		while(addr <= FRAM_ADDR_MAX){
			if(i>=len) break;
			by = data[i++];
			SpiSend(spi, &by, 1);
			addr++;
		}
		SpiDisableCS (spi);
		FramWriteDisable ();
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID FramInit(VOID)
{
	spi = BoardGetSpiFRam ();
	if (spi) {
		spi->bdrPrescaler = SPI_BAUDRATEPRESCALER_4; // 66M / 4 = 16M
		spi->firstBit     = SPI_FIRSTBIT_MSB;
		spi->clkPolarity  = SPI_POLARITY_LOW;	// CPOL = 0
		spi->clkPhase 	  = SPI_PHASE_1EDGE;	// CPHA = 0
		SpiInit (spi);
		BYTE status;
		INT16 cnt=0;
		do{
			status = FramReadStatus ();
			if(status != 0xFF) break;
			cnt ++;
			if(cnt > 10) break;
			HAL_Delay(10);
		}while(1);
		if(cnt > 10)
			DebugError(0, "Fram Init Error - Status=%02X", status);
		else
			DebugLog  (0, "Fram Init Finished. - (%02X)", status);
	} else {
		DebugError(0, "Fram Init Error - No Spi for Use.");
	}
}
/********************************************************************
 * End of File
 *******************************************************************/



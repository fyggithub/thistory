/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * OneWire.c
 *******************************************************************/
#include "OneWire.h"
#include "Console.h"
/********************************************************************
 *
 *******************************************************************/
#define ONEWIRE_IO		 GPIOB
#define ONEWIRE_PIN		 GPIO_PIN_7
#define ONEWIRE_MODEOOUT GPIO_MODE_OUTPUT_OD
#define ONEWIRE_MODEIN	 GPIO_MODE_INPUT
#define ONEWIRE_PULL	 GPIO_PULLUP
#define ONEWIRE_SPEED	 GPIO_SPEED_FREQ_HIGH
#define ONEWIRE_CLK_EN	 __HAL_RCC_GPIOB_CLK_ENABLE
/********************************************************************
 *
 *******************************************************************/


/********************************************************************
 *
 *******************************************************************/
STATIC VOID DelayUs(UINT16 us)
{
	us *= 7;
	for(; us>0; us--)
		__NOP();
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID OneWireOpen(VOID)
{
	ONEWIRE_CLK_EN();
	HAL_GPIO_WritePin (ONEWIRE_IO, ONEWIRE_PIN, GPIO_PIN_SET);

	GPIO_InitTypeDef init;
	init.Pin   = ONEWIRE_PIN;
	init.Mode  = ONEWIRE_MODEOOUT;
	init.Speed = ONEWIRE_SPEED;
	HAL_GPIO_Init(ONEWIRE_IO, &init);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID OneWireDioWrite(UINT16 v)
{
	if(v){
		HAL_GPIO_WritePin (ONEWIRE_IO, ONEWIRE_PIN, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin (ONEWIRE_IO, ONEWIRE_PIN, GPIO_PIN_RESET);
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT16 OneWireDioRead(VOID)
{
	UINT16 v = HAL_GPIO_ReadPin (ONEWIRE_IO, ONEWIRE_PIN);
	v = v != 0 ? 1 : 0;
	return v;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID OneWireWriteBit(UINT16 bit)
{
	//// Write Pulse < 15us
	OneWireDioWrite(0);
	DelayUs(10);

	//// Write Bit > 45us
	if(bit){
		OneWireDioWrite(1);
	}
	DelayUs(55);

	//// Wait > 1us
	OneWireDioWrite(1);
	DelayUs(10);
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT16 OneWireReadBit(VOID)
{
	//// Write Pulse > 1us
	OneWireDioWrite(0);
	DelayUs(3);

	//// Read Bit < 15us
	OneWireDioWrite(1);
	DelayUs(10);
	UINT16 v = OneWireDioRead();

	//// Delay > 45us && Change to Master && wait > 1us
	DelayUs(55);
	OneWireDioWrite(1);
	DelayUs(5);
	return v;
}
/********************************************************************
 *
 *******************************************************************/
UINT8 OneWireReadByte(VOID)
{
	UINT8 by = 0;
	for (INT16 i=0; i<8; i++){
		UINT16 b = OneWireReadBit();
		by >>= 1;
		by |= (b<<7);
	}
	return by;
}
/********************************************************************
 *
 *******************************************************************/
VOID OneWireWriteByte(UINT8 by)
{
	for (INT16 i=0; i<8; i++){
		UINT16 b = (by&0x01);
		OneWireWriteBit (b);
		by >>= 1;
	}
}
/********************************************************************
 *
 *******************************************************************/
BOOL OneWireReset(VOID)
{
	UINT16 v = 0;
	//// Pulse > 480us
	OneWireDioWrite (0);
	DelayUs(550);
	OneWireDioWrite (1);

	//// Wait Reset Pulse
	DelayUs(50);
	v = OneWireDioRead ();
	DelayUs(500);

	//// Retrun Success or Not
	return (v==0);
}
/********************************************************************
 *
 *******************************************************************/
BOOL OneWireInit(VOID)
{
	BOOL bFound = FALSE;
	bFound = FALSE;
	OneWireOpen ();
	bFound = OneWireReset();
	if (!bFound){
		DebugError(0, "1-Wire not Found!");
	}
	else {
		DebugLog(0, "1-Wire OK!");
	}
	return bFound;
}

/********************************************************************
 * END of File
 *******************************************************************/





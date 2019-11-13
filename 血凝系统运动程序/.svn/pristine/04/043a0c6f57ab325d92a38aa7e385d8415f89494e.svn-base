/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Config.c
 *******************************************************************/
#include "Config.h"
#include "Fram.h"
#include "Board.h"
#include "Bits.h"
#include "Console.h"
#include "stm32f1xx_hal.h"

CRC_HandleTypeDef crcHandle;
/********************************************************************
 *
 *******************************************************************/
VOID  ConfigInit(VOID)
{
	Message msg;
	FramInit();

	crcHandle.Instance = CRC;
	__HAL_RCC_CRC_CLK_ENABLE();
	HAL_CRC_Init(&crcHandle);

	if (! ConfigCheck () ){
		DebugError(0, "Config Found Error! Reset to Default!!!");
		ConfigResetDefault ();
		msg.id = MSG_CFG_RESET;
		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 ConfigGetWithoutRangeCheck(UINT16 id);

STATIC DWORD CalcCRC32(VOID)
{
	DWORD total = ConfigGetDword (CFG_TOTAL);
	DWORD crc = 0;
	for(UINT16 id = 1; id < total; id++){
		DWORD dword = (DWORD)ConfigGetWithoutRangeCheck(id);
		if (id==1)
			crc = HAL_CRC_Calculate (&crcHandle, &dword, 1);
		else
			crc = HAL_CRC_Accumulate(&crcHandle, &dword, 1);
	}
	return crc;
}

/********************************************************************
 *
 *******************************************************************/

BOOL  ConfigCheck(VOID)
{
	DWORD magic = ConfigGetDword (CFG_MAGIC);
	if ( magic == CONFIG_MAGIC){
		DWORD crcCalc = CalcCRC32 ();
		DWORD crcGet  = ConfigGetDword (CFG_CRC32);
		if (crcCalc == crcGet)
			return TRUE;
	}

	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/

VOID  ConfigResetDefault(VOID)
{
	UINT16 id = CFG_USER_BEGIN;
	UINT16 i;
	INT32 v;
	for(; id < CFG_MAX; id++){
		i = id-CFG_USER_BEGIN;
		v = configDefault[i].defaultv;
		ConfigSet(id, v);
	}

	ConfigSetDword (CFG_MAGIC,   CONFIG_MAGIC);
	ConfigSetDword (CFG_VERSION, CONFIG_VERSION);
	ConfigSetDword (CFG_TOTAL,   CFG_MAX);

	ConfigSetDone();
}

/********************************************************************
 *
 *******************************************************************/
STATIC INT32 CheckConfigRange(UINT16 id, INT32 v)
{
	UINT16 i;
	if (id >= CFG_USER_BEGIN && id < CFG_MAX){
		i = id-CFG_USER_BEGIN;
		INT32 min = configDefault[i].minv;
		INT32 max = configDefault[i].maxv;
		if( v < min || v > max)
			return configDefault[i].defaultv;
	}
	return v;
}

/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 ConfigGetWithoutRangeCheck(UINT16 id)
{
	if (id<CFG_MAX) {
		UINT16 addr = id*4;
		BYTE data[4];
		FramRead (addr, data, 4);
		UINT16 hword = MAKE_WORD (data[0], data[1]);
		UINT16 lword = MAKE_WORD (data[2], data[3]);
		UINT32 dword = MAKE_DWORD (hword, lword);
	    return (INT32)dword;
	}
	return 0;
}

INT32 ConfigGet(UINT16 id)
{
	INT32 v = ConfigGetWithoutRangeCheck(id);
	return CheckConfigRange (id, v);
}
/********************************************************************
 *
 *******************************************************************/
INT32 ConfigSet(UINT16 id, INT32 v)
{
	if (id<CFG_MAX) {
		UINT16 addr = id*4;
		BYTE data[4];

		v = CheckConfigRange (id, v);

		UINT32 dword = (UINT32)v;
		UINT16 hword = HWORD (dword);
		UINT16 lword = LWORD (dword);
		data[0] = HBYTE (hword);
		data[1] = LBYTE (hword);
		data[2] = HBYTE (lword);
		data[3] = LBYTE (lword);
		FramWrite (addr, data, 4);
		return v;
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/

DWORD ConfigGetDword(UINT16 id)
{
	return (DWORD)ConfigGet(id);
}
/********************************************************************
 *
 *******************************************************************/

DWORD ConfigSetDword(UINT16 id, DWORD dword)
{
	return (DWORD)ConfigSet(id, (INT32)dword);
}

/********************************************************************
 *
 *******************************************************************/

VOID  ConfigSetDone(VOID)
{
	DWORD crc = CalcCRC32 ();
	ConfigSetDword (CFG_CRC32, crc);
}

/********************************************************************
 * End of File
 *******************************************************************/



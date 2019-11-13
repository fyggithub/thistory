/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * DS18B20.c
 *******************************************************************/
#include "OneWire.h"
#include "DS18B20.h"
#include "Console.h"
#include "Bits.h"
/********************************************************************
 *
 *******************************************************************/
#define CMD_SEARCH	0xF0
#define CMD_READROM 0x33
#define CMD_MATCH   0x55
#define CMD_SKIP	0xCC
#define CMD_ALMSEARCH 0xEC
#define CMD_CONVERT 0x44
#define CMD_WRITE   0x4E
#define CMD_READ    0xBE
#define CMD_COPY    0x48
#define CMD_RECALL  0xB8
#define CMD_READPWR 0xB4
/********************************************************************
 *
 *******************************************************************/
STATIC CONST BYTE dscrc2x16_table[] = {
	0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
	0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
	0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
	0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
};
STATIC BYTE CalcCrc8(BYTE* data, UINT8 len)
{
	UINT8 crc = 0;

	while (len--) {
		crc = (*data++) ^ crc;  // just re-using crc as intermediate
		crc = (dscrc2x16_table [(crc & 0x0f)]) ^
			(dscrc2x16_table [ 16 + ((crc >> 4) & 0x0f)]);
	}

	return crc;
}
/********************************************************************
 *
 *******************************************************************/
VOID  TempSampleStart(VOID)
{
	OneWireReset ();
	OneWireWriteByte (CMD_SKIP);
	OneWireWriteByte (CMD_CONVERT);
	//DebugLog(0, "Temp Sample Convert");
}
/********************************************************************
 *
 *******************************************************************/
FLOAT TempSampleRead(VOID)
{
	BYTE data[9];
	OneWireReset ();
	OneWireWriteByte (CMD_SKIP);
	OneWireWriteByte (CMD_READ);
	for (INT16 i=0; i<9; i++){
		data[i] = OneWireReadByte();
	}
	INT32 celx16 = (INT16)(MAKE_WORD (data[1],data[0]));
	FLOAT cel = celx16/16.0;
	BYTE crc = CalcCrc8(data, 8);
	//DebugLog(0, "Temp Read Data: C=%.2f, CRC=%0X", cel, crc);
	//DebugHex(0, data, 9);

	if (crc == data[8])
		return cel;
	else
		return 15.55;
}
/********************************************************************
 *
 *******************************************************************/
VOID  TempSampleInfo(VOID)
{
	BYTE romCode[8];
	OneWireWriteByte (CMD_READROM);
	for(INT16 i=0; i<8; i++){
		romCode[i] = OneWireReadByte();
	}
	DebugLog(0, "Temp Sensor Rom Code:");
	DebugHex(0, romCode, 8);
}



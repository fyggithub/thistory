/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SerialDownLoad.h
 *******************************************************************/
#include "Datatypes.h"
#define HOST_CMD_UPDATE_ASK (0xAF)
#define HOST_CMD_UPDATE_TRAN (0xA7)
#define FLASH_BKP_START_ADDR (UINT32)0x08024000		//´úÂëÉý¼¶¶ÎµØÖ·
#define STM_SECTOR_SIZE 2048
#define NUM_BLOCK_BY_FRAM 8
/********************************************************************
 *
*******************************************************************/

 typedef struct _SerialLoad{
	UINT32	programLengthDiv32;
	UINT32	loadFrameNumber;
	UINT32	selfNumber;
	UINT8	upLoadingState;
	UINT32	programData[NUM_BLOCK_BY_FRAM];
	UINT32	first16Bytes[NUM_BLOCK_BY_FRAM];
	UINT32	addr;
 }SERIALLOAD;
/********************************************************************
 *
*******************************************************************/ 
enum{
	LOADREADY = 0,
	LOADBUSY,
	LOADWAIT,
};
/********************************************************************
*
*******************************************************************/
enum{
	LOAD_SUCCESS=0,
	LOAD_WAIT,
	LOAD_FAIL,
};
/********************************************************************
 *
*******************************************************************/
UINT8 SerialLoadStart(VOID);
VOID PrintSaveData(VOID);
VOID SerialLoadInit(UINT32 len);
VOID GetFileData(UINT8* data);
UINT8 GetLoadProgramState(VOID);
VOID LoadProgramStateInit(VOID);
/********************************************************************
 *
*******************************************************************/


/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SerialDownLoad.h
 *******************************************************************/
#include "Datatypes.h"

#ifdef STM32F103xE
#define FLASH_BKP_START_ADDR (UINT32)0x08024000		//代码升级段地址
#define STM_SECTOR_SIZE 2048
#else
#define FLASH_BKP_START_ADDR (UINT32)0x08014000		//代码升级段地址
#define STM_SECTOR_SIZE 1024
#endif

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
	UINT32	first32Bytes[NUM_BLOCK_BY_FRAM];
	UINT32	addr;
 }SERIALLOAD;
/********************************************************************
 *程序状态枚举
*******************************************************************/ 
enum{
	LOADREADY = 0,
	LOADBUSY,
	LOADWAIT,
};
/********************************************************************
*烧录结果枚举
*******************************************************************/
enum{
	LOAD_SUCCESS=0,
	LOAD_WAIT,
	LOAD_FAIL,
};
/********************************************************************
 *
*******************************************************************/
#define IAP_PROCESS_START \
	UINT8 loadProgramState = GetLoadProgramState();\
	if(loadProgramState == LOADWAIT){
/********************************************************************
 *
*******************************************************************/		
#define IAP_PROCESS_END \
	}\
	else{\
		switch(msg->id){\
			case MSG_HOST_UPDATEASK:\
			SerialLoadStart();\
			break;\
			case MSG_HOST_UPTRAN:\
			SerialLoadStart();\
			break;\
			default:\
			break;\
		}\
	}
/********************************************************************
 *
*******************************************************************/
#define CASE_HOST_CMD_IAP_TRAN \
	case HOST_CMD_UPDATE_TRAN	:	ret = HostCmdUpDateTran(frame); break;
	
#define CASE_HOST_CMD_IAP_ASK \
	case HOST_CMD_UPDATE_ASK	:	ret = HostCmdUpDateAsk(frame); break;

#define STATIC_BOOL_IAP_HOST_CMD_UPDATE_ASK_FUNCTION() \
STATIC BOOL HostCmdUpDateAsk(HostFrame* frame)\
{\
	Message msg;\
	if ( frame && frame->dataLen >= 1){\
		UINT32 len = (((UINT32)frame->data[0])<<24) + (((UINT32)frame->data[1])<<16) +\
			(((UINT32)frame->data[2])<<8) + frame->data[3];\
		SerialLoadInit(len);\
		msg.id = MSG_HOST_UPDATEASK;\
		MessagePost(&msg);\
		return TRUE;\
	}\
	return FALSE;\
}

#define STATIC_BOOL_IAP_HOST_CMD_UPDATE_TRAN_FUNCTION() \
STATIC BOOL HostCmdUpDateTran(HostFrame* frame)\
{\
	Message msg;\
	if ( frame && frame->dataLen >= 1){\
		GetFileData(frame->data);\
		msg.id = MSG_HOST_UPTRAN;\
		MessagePost(&msg);\
		return TRUE;\
	}\
	return FALSE;\
}	
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


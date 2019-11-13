/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SerialDownLoad.c
 *******************************************************************/

#include "SerialDownLoad.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "Host.h"
#include "Console.h"
/********************************************************************
*
*******************************************************************/
SERIALLOAD serialLoad;

/********************************************************************
*
*******************************************************************/
UINT8 GetLoadProgramState(VOID)
{
	return serialLoad.upLoadingState;
}
/********************************************************************
*
*******************************************************************/
VOID LoadProgramStateInit(VOID)
{
	serialLoad.upLoadingState = LOADWAIT;
}
/********************************************************************
*
*******************************************************************/
VOID SerialLoadInit(UINT32 len)
{
	serialLoad.addr = FLASH_BKP_START_ADDR;
	serialLoad.selfNumber = 0;
	serialLoad.upLoadingState = LOADREADY;

	UINT32 m = len%32;
	if(m != 0)
		serialLoad.programLengthDiv32 = len/32+1;
	else
		serialLoad.programLengthDiv32 = len/32;	
	
	HAL_FLASH_Unlock();	
	UINT32 secRemain = len%2048;
	UINT32 sectorSize = 0;
	FLASH_EraseInitTypeDef f;
	UINT32 pageerror = 0;
	f.TypeErase   = FLASH_TYPEERASE_PAGES;
	f.NbPages     = 1;	
	if(secRemain != 0)
		sectorSize = len/2048+1;
	else
		sectorSize = len/2048;
	for(UINT32 i=0; i<sectorSize; i++){		
		f.PageAddress = i*STM_SECTOR_SIZE+FLASH_BKP_START_ADDR;
		HAL_FLASHEx_Erase(&f, &pageerror);
	}
	HAL_FLASH_Lock();	
	SetHostOpenLogPra(FALSE);
	DebugLog    (0, "start");
}

/********************************************************************
*
*******************************************************************/
VOID GetFileData(UINT8* data)
{
	serialLoad.loadFrameNumber = (((UINT16)data[0])<<8) + data[1];
	for(UINT32 i=0; i<NUM_BLOCK_BY_FRAM; i++){
		serialLoad.programData[i] = (((UINT32)data[4*i+2])<<24) + (((UINT32)data[4*i+3])<<16) 
		+ (((UINT32)data[4*i+4])<<8) + (UINT32)data[4*i+5];
	}
}
/********************************************************************
*
*******************************************************************/
VOID SaveFileData(UINT32* sourceAddr)
{
	HAL_FLASH_Unlock();
	for(UINT8 i=0; i<NUM_BLOCK_BY_FRAM; i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, serialLoad.addr, sourceAddr[i]);
		serialLoad.addr = serialLoad.addr + NUM_BLOCK_BY_FRAM;
	}
	serialLoad.selfNumber++;
	HAL_FLASH_Lock();	
	return ;
}

/********************************************************************
*
*******************************************************************/
VOID SendLoadResultOrder(UINT8 result)
{
	UINT8 data[10];
	data[0] = result;
	data[1] = serialLoad.loadFrameNumber>>8;
	data[2] = serialLoad.loadFrameNumber;
	data[3] = 0x00;
	HostResponse(data,4);
}
/********************************************************************
*�����ļ�ͷдFFFF����󽫰汾�ż�У����Ϣд���ļ�ͷ
*******************************************************************/
UINT8 SavePrograme(VOID)
{	
	UINT16 t = serialLoad.loadFrameNumber;
	if(t == 0){
		for(UINT8 i=0; i<NUM_BLOCK_BY_FRAM; i++){
			serialLoad.first16Bytes[i] = serialLoad.programData[i];
			serialLoad.addr = serialLoad.addr + NUM_BLOCK_BY_FRAM;
		}
		serialLoad.selfNumber++;		
		if((t+1) == serialLoad.programLengthDiv32){
			serialLoad.addr = FLASH_BKP_START_ADDR;
			SaveFileData(serialLoad.first16Bytes);		
		}
		SendLoadResultOrder(LOAD_SUCCESS);
		return LOAD_SUCCESS;
	}
	if(t != serialLoad.selfNumber){
		SendLoadResultOrder(LOAD_FAIL);
		return LOAD_FAIL;
	}
	else if((t+1) == serialLoad.programLengthDiv32){
		SaveFileData(serialLoad.programData);
		serialLoad.addr = FLASH_BKP_START_ADDR;
		SaveFileData(serialLoad.first16Bytes);
		SendLoadResultOrder(LOAD_SUCCESS);
		LoadProgramStateInit();	
		DebugLog    (0, "finish");
	}
	else{
		SaveFileData(serialLoad.programData);
		SendLoadResultOrder(LOAD_SUCCESS);
	}
	return LOAD_SUCCESS;
}

/********************************************************************
*
*******************************************************************/
UINT8 SerialLoadStart(VOID)
{
	UINT8 su = serialLoad.upLoadingState;
	UINT8 t = LOAD_FAIL;
	switch(su){
		case LOADBUSY:
			t = SavePrograme();
			if((serialLoad.loadFrameNumber+1) >
				(serialLoad.programLengthDiv32))
				serialLoad.upLoadingState = LOADREADY;
			break;
		case LOADREADY:{
				serialLoad.upLoadingState = LOADBUSY;
				UINT8 data[1] = {0x00};
				HostResponse(data,1);
				t = LOAD_SUCCESS;
			}
			break;
		default:
			SendLoadResultOrder(LOAD_WAIT);		
			break;
	}
	return t;
}

/********************************************************************
*NUM_BLOCK_BY_FRAM=4
*******************************************************************/
VOID PrintSaveData(VOID)
{
	UINT32 addr = FLASH_BKP_START_ADDR;
	UINT8 data[4];
	UINT16 len = serialLoad.programLengthDiv32*32;
	for(UINT16 i=0; i<len; i++){
		data[0] = (*(__IO UINT32*)addr)>>24;
		data[1] = (*(__IO UINT32*)addr)>>16;
		data[2] = (*(__IO UINT32*)addr)>>8;
		data[3] = (*(__IO UINT32*)addr);
		addr += 4;
		DebugHex(0,(UINT8*)data,4);
	}
	return ;
}
/********************************************************************
*
*******************************************************************/

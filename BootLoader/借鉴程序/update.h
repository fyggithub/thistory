
#ifndef __UPDATE_H__
#define __UPDATE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "CoreTypes.h"

//出厂设置默认程序
#define FILE_LEN_ADDR_DEFAULT			(0x1FF000)		//Flash保护区2M中的最后一个扇区4096开始位置		
#define	FILE_START_ADDR_DEFAULT			(0)				//程序存储在Flash开始处
//用户下载升级程序
#define FILE_LEN_ADDR_USER				(0x1FFF000)		//Flash保护区最后2M中的最后一个扇区4096开始位置		
#define	FILE_START_ADDR_USER			(0x1E00000)		//程序存储在Flash开始处	
		
#define UPGRATE_FLAG_ADDR				(0x1FFF004)		//Flash保护区最后2M中的最后一个扇区offset 4字节位置	
#define UPGRATE_FLAG_VALUE				(0xccee55aa)	//升级用户下载应用程序标记	
#define	UPGRATE_FLAG_CLEAR				(0xFFFFFFFF)
	
#define UPGRATE_RESULT_ADDR				(0)				//升级结果标记保存在备份SRAM的起始地址处，大小为一个字节

#define EEPROM_FLAG_ADDR				(0)				//EEPROM中保存出厂设置标记的地址	
	
//升级结果标记值
enum{
	UPGRATE_INIT = 0,
	UPGRATE_OK=1,
	UPGRATE_CRC_FAIL,
	UPGRATE_EEPROM_FAIL,
	UPGRATE_FACTORY_OK,
	UPGRATE_USER_OK,
};	
	
BOOL SetUpgrateResultAndFlag(UINT32 Flag,UINT8 Result);	
BOOL CheckIfUpgrateFactoryCode(VOID);	
BOOL CheckUpgrateFileCrc(VOID);
BOOL CheckIfNeedUpgrateFile(VOID);	
BOOL StartUpgrateProcess(VOID);	
VOID JumpToRunUserCode(VOID);
BOOL GetUpgrateFileLen(UINT32 * FileLen);
BOOL CheckIfRestoreFactoryKeyPressed(VOID);
BOOL CheckIfUpgrateFromSd(VOID);
BOOL CheckIfCopyFactotyFileToFlash(VOID);
#ifdef __cplusplus
}
#endif
#endif //__UPDATE_H__



#include "stm32f4xx.h"
#include "update.h"
#include "DriverFlash.h"
#include "DriverCrc.h"
#include "DriverIap.h"
#include "DriverKeyEncode.h"
#include "DriverBackupSram.h"
#include "DriverEeprom.h"
#include "DriverLcd.h"
#include "fonts.h"
#include <string.h>
#if DEBUG_TRACE==1
	#include <stdio.h>
#endif

//保存升级文件长度值，在Flash中保存的地址
volatile UINT32 gUpgrateFileLenAddr = FILE_LEN_ADDR_USER;
//保存升级文件在Flash中起始地址
volatile UINT32 gUpgrateFileStartAddr = FILE_START_ADDR_USER;

//恢复出厂设置程序后，用于清除EEPROM非保护区内容
static BOOL SetEepromToFactoryDefault(VOID)
{
	UINT8 tempData[4096] = {0};
	UINT8 SectorCount = 0;
	DisplayUpdateProcess((UINT8 *)"erase eeprom flash bram");
	memset(tempData,0xFF,4096);
	
	for(SectorCount = 0;SectorCount<EEPROM_PAGE_NUM-16;SectorCount++)
	{
		if(DriverEepromWrite(tempData,SectorCount<<6,EEPROM_PAGE_SIZE)!=EEPROM_PAGE_SIZE)
		{
			DisplayErrorMessage((UINT8 *)"erase eeprom error");
			return FALSE;
		}
	}
	if(DriverFlashSectorWrite(tempData,0,4096)!=4096)
	{
		DisplayErrorMessage((UINT8 *)"erase flash error");
			return FALSE;
	}
	if(DriverBackupSramWrite(1,tempData,4095)!=4095)
	{
		DisplayErrorMessage((UINT8 *)"erase bsram error");
			return FALSE;
	}
	return TRUE;
	

}

BOOL CheckIfUpgrateFactoryCode(VOID)
{
	if(gUpgrateFileLenAddr == FILE_LEN_ADDR_DEFAULT)
		return TRUE;
	else
		return FALSE;
}

BOOL GetUpgrateFileLen(UINT32 * FileLen)
{
	UINT32 Len = 0;
	if(DriverFlashBufferRead((UINT8 *)(&Len),gUpgrateFileLenAddr,4)==4)
	{
		*FileLen = Len;
		return TRUE;
	}
	else
		return FALSE;	
}
BOOL CheckUpgrateFileCrc(VOID)
{
	UINT8 date[Flash_SectorSize]={0};
	UINT32 FileLen = 0,TempCrc = 0,StartSector=0,CrcValueInFile=0;
	UINT32 SectorNum=0,SectorCount=0,SingleCount = 0,TempLen = Flash_SectorSize;
	
	if(!GetUpgrateFileLen(&FileLen))
		return FALSE;
	//升级文件长度必须是字对齐的
	if(FileLen%4!=0)
		return FALSE;
	//文件长度减去CRC校验的四字节数
	FileLen -= 4;
	//计算Flash中升级文件占用的扇区数
	SectorNum	= FileLen/Flash_SectorSize;
	SingleCount = FileLen%Flash_SectorSize;
	//升级文件起始扇区编号
	StartSector = gUpgrateFileStartAddr/Flash_SectorSize;
	//先将Flash中完整扇区的内容进行校验
	CRC_ResetDR();
	for(SectorCount=0;SectorCount<SectorNum;SectorCount++)
	{
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
			return FALSE;
		if(DriverCrcCalculate(date,TempLen,&TempCrc)==FALSE)
			return FALSE;
	}
	//将不足一个扇区的内容copy到ROM中
	if(SingleCount!=0)
	{
		TempLen = SingleCount;
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen+4)!=(TempLen+4))
			return FALSE;
		if(DriverCrcCalculate(date,TempLen,&TempCrc)==FALSE)
			return FALSE;
		//获取升级文件中CRC校验值，保存在升级文件的最后四个字节
		CrcValueInFile = *((UINT32 *)(&date[TempLen]));
	}
	else
	{
		//CRC存在新的扇区中
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,4)!=4)
			return FALSE;
		CrcValueInFile = *((UINT32 *)(&date[0]));;
	}
	
	if(TempCrc==CrcValueInFile)
		return TRUE;
	else
		return FALSE;
		
}
BOOL CheckIfNeedUpgrateFile(VOID)
{
	UINT32 FlagValue = 0;

	if(DriverFlashBufferRead((UINT8 *)(&FlagValue),UPGRATE_FLAG_ADDR,4)!=4)
	{
		//如果Flash读取失败，则重复读取一次，依然失败，则返回
		if(DriverFlashBufferRead((UINT8 *)(&FlagValue),UPGRATE_FLAG_ADDR,4)!=4)
			return FALSE;
	}
			
	if(FlagValue == UPGRATE_FLAG_VALUE)
	{
		gUpgrateFileLenAddr = FILE_LEN_ADDR_USER;
		gUpgrateFileStartAddr = FILE_START_ADDR_USER;
		return TRUE;
	}
	
	return FALSE;
}
BOOL SetUpgrateResultAndFlag(UINT32 Flag,UINT8 Result)
{
	UINT32 FileLen[2] = {0,Flag};
	UINT8 UpgrateResult = Result;
	//清除升级标记，升级标记和文件长度保存在Flash最后4K地址处
	if(DriverFlashBufferRead((UINT8 *)(&FileLen),FILE_LEN_ADDR_USER,4)!=4)
		return FALSE;
	if(DriverFlashProtectSectorWrite((UINT8 *)FileLen,FILE_LEN_ADDR_USER/Flash_SectorSize,8)!=8)
		return FALSE;
	//在备份SRAM中设置升级结果标记
	if(DriverBackupSramWrite(UPGRATE_RESULT_ADDR,&UpgrateResult,1)!=1)
		return FALSE;
	return TRUE;
	
}
BOOL StartUpgrateProcess(VOID)
{
	UINT8 date[Flash_SectorSize]={0};
	UINT32 FileLen = 0,TempLen = Flash_SectorSize,SectorNum=0,SectorCount=0,SingleCount = 0;
	UINT32 RomAddr = FLASH_USER_START_ADDR,StartSector=0;
	UINT32 CrcValueInRom = 0,CrcValueInFile;
	UINT8 DisplayFlag = 0;
	
	if(!GetUpgrateFileLen(&FileLen))
		return FALSE;
	//取消写保护
	if(!DriverDisableFlashWriteProtection(FLASH1_PROTCXT_SECTOR_NUM,FLASH2_PROTCXT_SECTOR_NUM))
		return FALSE;
	if(!DriverIapErase(FLASH_USER_START_ADDR,FLASH_USER_END_ADDR))
	{
		DisplayErrorMessage((UINT8 *)"erase rom error");
		return FALSE;
	}
	//计算Flash中升级文件占用的扇区数
	SectorNum	= FileLen/Flash_SectorSize;
	SingleCount = FileLen%Flash_SectorSize;
	//升级文件在Flash中的起始扇区编号
	StartSector = gUpgrateFileStartAddr/Flash_SectorSize;
	//进度条显示
	DisplayUpdateProcess((UINT8 *)string_program_off);
	//先将Flash中完整扇区的内容copy到ROM中
	for(SectorCount=0;SectorCount<SectorNum;SectorCount++)
	{
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
		{
			DisplayErrorMessage((UINT8 *)"file read error");
			return FALSE;
		}
		//Flash中数据是经过加密处理的，将原有4K程序前后2k进行了调换
		if(DriverIapWrite(RomAddr,(UINT32 *)(&date[TempLen/2]),TempLen>>3)==FALSE)
		{
			DisplayErrorMessage((UINT8 *)"iap write error");
			return FALSE;
		}
		if(DriverIapWrite(RomAddr+TempLen/2,(UINT32 *)(date),TempLen>>3)==FALSE)
		{
			DisplayErrorMessage((UINT8 *)"iap write error");
			return FALSE;
		}
		RomAddr += TempLen;	
		
		if(DisplayFlag ==0)
		{
			DisplayUpdateProcess((UINT8 *)string_program_on);
			DisplayFlag = 1;
		}
		else
		{
			DisplayUpdateProcess((UINT8 *)string_program_off);
			DisplayFlag = 0;
		}
	}
	//将不足一个扇区的内容copy到ROM中
	if(SingleCount!=0)
	{
		TempLen = SingleCount;
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
			return FALSE;
		if(DriverIapWrite(RomAddr,(UINT32 *)date,TempLen>>2)==FALSE)
			return FALSE;
		RomAddr += TempLen;	
	}
	//解密后的文件，最后8个字节都是CRC值，前四字节为原始文件的CRC值，后四字节为加密后的CRC值
	//读取原始文件的CRC值
	CrcValueInFile = *(__IO UINT32*)(FLASH_USER_START_ADDR+FileLen-8);
	//升级结束后，计算CRC内容,每次启动CRC计算前需要复位硬件CRC值
	CRC_ResetDR();

	FileLen -=8;
	SectorNum	= FileLen/Flash_SectorSize;
	SingleCount = FileLen%Flash_SectorSize;
	for(SectorCount=0;SectorCount<SectorNum;SectorCount++)
	{
		DriverCrcCalculate((UINT8 *)(FLASH_USER_START_ADDR+SectorCount*Flash_SectorSize),Flash_SectorSize,&CrcValueInRom);
	}
	if(SingleCount!=0)
	{
		DriverCrcCalculate((UINT8 *)(FLASH_USER_START_ADDR+SectorCount*Flash_SectorSize),SingleCount,&CrcValueInRom);
	}
	
	if(CrcValueInRom !=CrcValueInFile)
	{
	#if DEBUG_TRACE==1
		printf("\r\nCrc Error In Rom:CrcValueInFile=0x%x,CrcValueInRom = 0x%x",CrcValueInFile,CrcValueInRom);
	#endif
		DisplayErrorMessage((UINT8 *)"crc error in rom");
		return FALSE;
	}
	//CRC校验通过后，清除升级标志位
	if(CheckIfUpgrateFactoryCode())
	{
		//清除EEPROM
		UINT32 count = 0;
		for(count=0;count<5;count++)
		{
			if(SetEepromToFactoryDefault())
				break;
		}
		if(count>=5)
		{
		#if DEBUG_TRACE==1
			printf("\r\n Eraase EEPROM Fail");
		#endif
			SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_EEPROM_FAIL);
		}
		else
			SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_FACTORY_OK);
	}
	else
	{
		SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_USER_OK);
	}
	//设置写保护,设置写保护暂时不调用，一旦调用，会给Debug调试带来不便，正式程序将添加
	
	//重启设备
	NVIC_SystemReset();
	//正常情况下不会执行该return
	return TRUE;
	
}

typedef  void (*pFunction)(void);
VOID JumpToRunUserCode(VOID)
{
	UINT32 JumpAddress=0;
	pFunction Jump_To_Application;
	JumpAddress = *(__IO UINT32*) (FLASH_USER_START_ADDR + 4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(__IO UINT32*) FLASH_USER_START_ADDR);
	Jump_To_Application();
}



static UINT32 gTimerCount = 0;

BOOL CheckIfRestoreFactoryKeyPressed(VOID)
{
	BOOL TimerStartFlag = FALSE;
	while((KEY_STANDBY_INT_IN==0)&&(KEY_RAMP_INT_IN==0)&&(Encode_S_INT_IN==0))
	{
		//开启定时器，5s计时开始,1ms中断
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//停止定时器
			SysTick->CTRL  &= ~(0x1);
			
			gUpgrateFileLenAddr = FILE_LEN_ADDR_DEFAULT;
			gUpgrateFileStartAddr = FILE_START_ADDR_DEFAULT;
			return TRUE;
		}
		
	}
	
	return FALSE;

}

BOOL CheckIfUpgrateFromSd(VOID)
{
	BOOL TimerStartFlag = FALSE;
	
	while((KEY_STANDBY_INT_IN==0)&&(KEY_RAMP_INT_IN==1)&&(Encode_S_INT_IN==0))
	{
		//开启定时器，5s计时开始,1ms中断
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//停止定时器
			SysTick->CTRL  &= ~(0x1);
			
			gUpgrateFileLenAddr = FILE_LEN_ADDR_DEFAULT;
			gUpgrateFileStartAddr = FILE_START_ADDR_DEFAULT;
			return TRUE;
		}
		
	}
	
	return FALSE;

}

BOOL CheckIfCopyFactotyFileToFlash(VOID)
{
	BOOL TimerStartFlag = FALSE;
	
	while((KEY_STANDBY_INT_IN==1)&&(KEY_RAMP_INT_IN==0)&&(Encode_S_INT_IN==0))
	{
		//开启定时器，5s计时开始,1ms中断
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//停止定时器
			SysTick->CTRL  &= ~(0x1);
			
			gUpgrateFileLenAddr = FILE_LEN_ADDR_DEFAULT;
			gUpgrateFileStartAddr = FILE_START_ADDR_DEFAULT;
			return TRUE;
		}
		
	}
	
	return FALSE;

}

void SysTick_Handler(void)
{
	gTimerCount++;
}




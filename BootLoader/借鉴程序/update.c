
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

//���������ļ�����ֵ����Flash�б���ĵ�ַ
volatile UINT32 gUpgrateFileLenAddr = FILE_LEN_ADDR_USER;
//���������ļ���Flash����ʼ��ַ
volatile UINT32 gUpgrateFileStartAddr = FILE_START_ADDR_USER;

//�ָ��������ó�����������EEPROM�Ǳ���������
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
	//�����ļ����ȱ������ֶ����
	if(FileLen%4!=0)
		return FALSE;
	//�ļ����ȼ�ȥCRCУ������ֽ���
	FileLen -= 4;
	//����Flash�������ļ�ռ�õ�������
	SectorNum	= FileLen/Flash_SectorSize;
	SingleCount = FileLen%Flash_SectorSize;
	//�����ļ���ʼ�������
	StartSector = gUpgrateFileStartAddr/Flash_SectorSize;
	//�Ƚ�Flash���������������ݽ���У��
	CRC_ResetDR();
	for(SectorCount=0;SectorCount<SectorNum;SectorCount++)
	{
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
			return FALSE;
		if(DriverCrcCalculate(date,TempLen,&TempCrc)==FALSE)
			return FALSE;
	}
	//������һ������������copy��ROM��
	if(SingleCount!=0)
	{
		TempLen = SingleCount;
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen+4)!=(TempLen+4))
			return FALSE;
		if(DriverCrcCalculate(date,TempLen,&TempCrc)==FALSE)
			return FALSE;
		//��ȡ�����ļ���CRCУ��ֵ�������������ļ�������ĸ��ֽ�
		CrcValueInFile = *((UINT32 *)(&date[TempLen]));
	}
	else
	{
		//CRC�����µ�������
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
		//���Flash��ȡʧ�ܣ����ظ���ȡһ�Σ���Ȼʧ�ܣ��򷵻�
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
	//���������ǣ�������Ǻ��ļ����ȱ�����Flash���4K��ַ��
	if(DriverFlashBufferRead((UINT8 *)(&FileLen),FILE_LEN_ADDR_USER,4)!=4)
		return FALSE;
	if(DriverFlashProtectSectorWrite((UINT8 *)FileLen,FILE_LEN_ADDR_USER/Flash_SectorSize,8)!=8)
		return FALSE;
	//�ڱ���SRAM����������������
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
	//ȡ��д����
	if(!DriverDisableFlashWriteProtection(FLASH1_PROTCXT_SECTOR_NUM,FLASH2_PROTCXT_SECTOR_NUM))
		return FALSE;
	if(!DriverIapErase(FLASH_USER_START_ADDR,FLASH_USER_END_ADDR))
	{
		DisplayErrorMessage((UINT8 *)"erase rom error");
		return FALSE;
	}
	//����Flash�������ļ�ռ�õ�������
	SectorNum	= FileLen/Flash_SectorSize;
	SingleCount = FileLen%Flash_SectorSize;
	//�����ļ���Flash�е���ʼ�������
	StartSector = gUpgrateFileStartAddr/Flash_SectorSize;
	//��������ʾ
	DisplayUpdateProcess((UINT8 *)string_program_off);
	//�Ƚ�Flash����������������copy��ROM��
	for(SectorCount=0;SectorCount<SectorNum;SectorCount++)
	{
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
		{
			DisplayErrorMessage((UINT8 *)"file read error");
			return FALSE;
		}
		//Flash�������Ǿ������ܴ���ģ���ԭ��4K����ǰ��2k�����˵���
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
	//������һ������������copy��ROM��
	if(SingleCount!=0)
	{
		TempLen = SingleCount;
		if(DriverFlashProtectSectorRead(date,SectorCount+StartSector,TempLen)!=TempLen)
			return FALSE;
		if(DriverIapWrite(RomAddr,(UINT32 *)date,TempLen>>2)==FALSE)
			return FALSE;
		RomAddr += TempLen;	
	}
	//���ܺ���ļ������8���ֽڶ���CRCֵ��ǰ���ֽ�Ϊԭʼ�ļ���CRCֵ�������ֽ�Ϊ���ܺ��CRCֵ
	//��ȡԭʼ�ļ���CRCֵ
	CrcValueInFile = *(__IO UINT32*)(FLASH_USER_START_ADDR+FileLen-8);
	//���������󣬼���CRC����,ÿ������CRC����ǰ��Ҫ��λӲ��CRCֵ
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
	//CRCУ��ͨ�������������־λ
	if(CheckIfUpgrateFactoryCode())
	{
		//���EEPROM
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
	//����д����,����д������ʱ�����ã�һ�����ã����Debug���Դ������㣬��ʽ�������
	
	//�����豸
	NVIC_SystemReset();
	//��������²���ִ�и�return
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
		//������ʱ����5s��ʱ��ʼ,1ms�ж�
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//ֹͣ��ʱ��
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
		//������ʱ����5s��ʱ��ʼ,1ms�ж�
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//ֹͣ��ʱ��
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
		//������ʱ����5s��ʱ��ʼ,1ms�ж�
		if(TimerStartFlag==FALSE)
		{
			SysTick_Config(SystemCoreClock /1000);
			TimerStartFlag = TRUE;
			gTimerCount = 0;
		}
		if(gTimerCount>=5000)
		{
			//ֹͣ��ʱ��
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




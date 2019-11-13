
#include "stm32f4xx.h"
#include "DriverIap.h"
#include "DriverLcd.h"
#include "fonts.h"
#include <stdio.h>

static UINT32 GetSector(UINT32 Address)
{
	UINT32 sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;  
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;  
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;  
	}
	else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
	{
		sector = FLASH_Sector_11;  
	}

	else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
	{
		sector = FLASH_Sector_12;  
	}
	else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
	{
		sector = FLASH_Sector_13;  
	}
	else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
	{
		sector = FLASH_Sector_14;  
	}
	else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
	{
		sector = FLASH_Sector_15;  
	}
	else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
	{
		sector = FLASH_Sector_16;  
	}
	else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
	{
		sector = FLASH_Sector_17;  
	}
	else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
	{
		sector = FLASH_Sector_18;  
	}
	else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
	{
		sector = FLASH_Sector_19;  
	}
	else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
	{
		sector = FLASH_Sector_20;  
	} 
	else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
	{
		sector = FLASH_Sector_21;  
	}
	else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
	{
		sector = FLASH_Sector_22;  
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
	{
		sector = FLASH_Sector_23;  
	}
	return sector;
}

BOOL DriverIapErase(UINT32 StartAddr,UINT32 EndAddr)
{
	UINT32 StartSectorNum = 0,EndSectorNum = 0,SectorCount = 0;
	UINT8 DisplayFlag = 0;
	
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	StartSectorNum = GetSector(StartAddr);
	EndSectorNum = GetSector(EndAddr);
	SectorCount	= StartSectorNum;
	
	DisplayUpdateProcess((UINT8 *)string_erase_off);
	
	FLASH_Unlock();
	while (SectorCount <= EndSectorNum) 
	{
		//使用2.7~3.5V电压
		if (FLASH_EraseSector(SectorCount, VoltageRange_3) != FLASH_COMPLETE)
		{ 
		#if DEBUG_TRACE==1
			printf("\r\nSector Erase Error,SectorNum=0x%x",SectorCount);
		#endif
			FLASH_Lock();
			return FALSE;
		}
		if (SectorCount == FLASH_Sector_11)
		{
			SectorCount += 40;
		} 
		else 
		{
			SectorCount += 8;
		}
		if(DisplayFlag ==0)
		{
			DisplayUpdateProcess((UINT8 *)string_erase_on);
			DisplayFlag = 1;
		}
		else
		{
			DisplayUpdateProcess((UINT8 *)string_erase_off);
			DisplayFlag = 0;
		}
	#if DEBUG_TRACE==1
		printf("\r\nErasing...... ");
	#endif
	}
	FLASH_Lock();
	return TRUE;
}
//WriteLen单位为字，四字节
BOOL DriverIapWrite(UINT32 WriteAddr,UINT32* pBuff,UINT32 WriteLen)
{
	UINT32 addr = WriteAddr;
	if(pBuff==NULL || WriteLen==0)
	{
	#if DEBUG_TRACE==1
		printf("\r\nIap Write Paremeter Error");
	#endif
		return FALSE;
	}
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	while(WriteLen>0)
	{
		if (FLASH_ProgramWord(addr, *pBuff) == FLASH_COMPLETE)
		{
			addr += 4;
			pBuff++;
			WriteLen--;
		}
		else
		{ 
		#if DEBUG_TRACE==1
			printf("\r\nIAP Write word Error");
		#endif
			FLASH_Lock();
			return FALSE;
		}
	}
	FLASH_Lock();
	return TRUE;
}

BOOL DriverDisableFlashWriteProtection(UINT16 Flash1SectorNums,UINT16 Flash2SectorNums)
{
	UINT32 ProtectStatus = 0;
	UINT32 SectorNums = Flash1SectorNums + (Flash2SectorNums<<16);
	
	ProtectStatus = FLASH_OB_GetWRP()+ (FLASH_OB_GetWRP1()<<16);
	
	ProtectStatus = ProtectStatus&SectorNums;
	//需要禁止写保护的扇区中，有写保护
	if(ProtectStatus!=SectorNums)
	{
		FLASH_OB_Unlock();
		FLASH_OB_WRPConfig(Flash1SectorNums, DISABLE);
		FLASH_OB_WRP1Config(Flash2SectorNums, DISABLE);
		if (FLASH_OB_Launch() != FLASH_COMPLETE)
		{
			FLASH_OB_Lock();
			return FALSE;
		}
		FLASH_OB_Lock();
		
		ProtectStatus = FLASH_OB_GetWRP()+ (FLASH_OB_GetWRP1()<<16);
		ProtectStatus = ProtectStatus&SectorNums;
		if(ProtectStatus==SectorNums)
			return TRUE;
		else
			return FALSE;
		
	}
	
	return TRUE;
	
}
BOOL DriverEnableFlashWriteProtection(UINT16 Flash1SectorNums,UINT16 Flash2SectorNums)
{
	UINT32 ProtectStatus = 0;
	UINT32 SectorNums = Flash1SectorNums + (Flash2SectorNums<<16);
	
	ProtectStatus = FLASH_OB_GetWRP()+ (FLASH_OB_GetWRP1()<<16);
	
	ProtectStatus = ProtectStatus&SectorNums;
	
	if(ProtectStatus!=0)
	{
		FLASH_OB_Unlock();
		FLASH_OB_WRPConfig(Flash1SectorNums, ENABLE);
		FLASH_OB_WRP1Config(Flash2SectorNums, ENABLE);
		if (FLASH_OB_Launch() != FLASH_COMPLETE)
		{
			FLASH_OB_Lock();
			return FALSE;
		}
		FLASH_OB_Lock();
		
		ProtectStatus = FLASH_OB_GetWRP()+ (FLASH_OB_GetWRP1()<<16);
		ProtectStatus = ProtectStatus&SectorNums;
		if(ProtectStatus==0)
			return TRUE;
		else
			return FALSE;
	
	}
	
	return TRUE;

}


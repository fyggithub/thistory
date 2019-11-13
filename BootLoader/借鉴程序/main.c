
#include "stm32f4xx.h"
#include "CoreTypes.h"
#include "DriverUart.h"
#include "DriverDelay.h"
#include "DriverIap.h"
#include "DriverFlash.h"
#include "DriverCrc.h"
#include "DriverSd.h"
#include "DriverKeyEncode.h"
#include "DriverEeprom.h"
#include "DriverBackupSram.h"
#include "DriverLcd.h"
#include "update.h"
#include "tempSd.h"
#include "fonts.h"
#if DEBUG_TRACE==1
	#include <stdio.h>
#endif

//保存升级文件长度值，在Flash中保存的地址
extern volatile UINT32 gUpgrateFileLenAddr;
//保存升级文件在Flash中起始地址
extern volatile UINT32 gUpgrateFileStartAddr;


INT32 main(VOID)
{
	UINT8 UpgrateCount = 0;
	// 优先级分组设置为4，可配置0-15级抢占式优先级，0级子优先级，即不存在子优先级。
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// 根据PLL配置更新系统时钟频率变量 SystemCoreClock
	SystemCoreClockUpdate();
	// 设置systick时钟源
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
#if DEBUG_TRACE==1
	DriverUSART1Init(115200);
	printf("\r\nuart test\r\n");
	printf("sys main frequency = %d\r\n",SystemCoreClock);
#endif
	
	DriverDelayInit();
	DriverFlashInit();
	DriverKeyEncodeInit();
	DriverInitBackupSram();
	
//	//添加从SD卡读取升级文件，方便研发调试，后续会移除,Encode 与Standby按键按下超过5s
//	if(CheckIfUpgrateFromSd())
//	{	
//		DriverLcdInit();
//	#if DEBUG_TRACE==1
//		printf("\r\nstart copy file from SD to Flash");
//	#endif
//		//文件成功复制到Flash中
//		if(CopyFileFromSdToFlashUser(FILE_START_ADDR_USER,FILE_LEN_ADDR_USER))
//		{
//			//设置升级标志
//			SetUpgrateResultAndFlag(UPGRATE_FLAG_VALUE,UPGRATE_INIT);
//		}
//	}
//	//添加从SD卡copy出厂设置程序到Flash指定位置，后续会移除
//	if(CheckIfCopyFactotyFileToFlash())
//	{
//		DriverLcdInit();
//	#if DEBUG_TRACE==1
//		printf("\r\nstart copy factory file from SD to Flash");
//	#endif
//		CopyFileFromSdToFlashDefault(FILE_START_ADDR_DEFAULT,FILE_LEN_ADDR_DEFAULT);
//	}
	
	//不恢复出厂设置程序，也没有升级标志
	if(!CheckIfRestoreFactoryKeyPressed())
	{
		if(!CheckIfNeedUpgrateFile())
		{
//			SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_OK);
			JumpToRunUserCode();
		}			
	}
	//需要开始升级操作	
	DriverCrcInit();
	DriverEepromInit();
	DriverLcdInit();

	if(CheckUpgrateFileCrc())
	{
		//如果升级的是出厂设置程序，则在开始的时候设置升级标志，目的主要是防止在升级出厂 程序过程中，如果断电导致升级失败
		//则在重新上电后，能够加载用户程序
		if(CheckIfUpgrateFactoryCode())
		{
			SetUpgrateResultAndFlag(UPGRATE_FLAG_VALUE,UPGRATE_INIT);
		}
		
		//升级3次失败
		UpgrateCount = 0;
		while(UpgrateCount < 3)
		{
			//如果升级成功，程序间更不会返回此处，直接重启系统，跳转应用程序
			if(StartUpgrateProcess()==FALSE)
			{
				UpgrateCount++;
			}
		}
		//如果升级的程序不是出厂设置程序，需要恢复出厂设置程序
		if(!CheckIfUpgrateFactoryCode())
		{
			gUpgrateFileLenAddr = FILE_LEN_ADDR_DEFAULT;
			gUpgrateFileStartAddr = FILE_START_ADDR_DEFAULT;
			if(CheckUpgrateFileCrc())
			{
				UpgrateCount = 0;
				while(UpgrateCount < 3)
				{
					//如果升级成功，程序间更不会返回此处，直接重启系统，跳转应用程序
					if(StartUpgrateProcess()==FALSE)
					{
						UpgrateCount++;
					}
				}
			}
			else
				DisplayErrorMessage((UINT8 *)"file crc error");
			
		}
				
	}
	else
	{
		SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_CRC_FAIL);
		DisplayErrorMessage((UINT8 *)"file crc error");
		JumpToRunUserCode();
	}
	while(1);

}







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

//���������ļ�����ֵ����Flash�б���ĵ�ַ
extern volatile UINT32 gUpgrateFileLenAddr;
//���������ļ���Flash����ʼ��ַ
extern volatile UINT32 gUpgrateFileStartAddr;


INT32 main(VOID)
{
	UINT8 UpgrateCount = 0;
	// ���ȼ���������Ϊ4��������0-15����ռʽ���ȼ���0�������ȼ����������������ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// ����PLL���ø���ϵͳʱ��Ƶ�ʱ��� SystemCoreClock
	SystemCoreClockUpdate();
	// ����systickʱ��Դ
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
	
//	//��Ӵ�SD����ȡ�����ļ��������з����ԣ��������Ƴ�,Encode ��Standby�������³���5s
//	if(CheckIfUpgrateFromSd())
//	{	
//		DriverLcdInit();
//	#if DEBUG_TRACE==1
//		printf("\r\nstart copy file from SD to Flash");
//	#endif
//		//�ļ��ɹ����Ƶ�Flash��
//		if(CopyFileFromSdToFlashUser(FILE_START_ADDR_USER,FILE_LEN_ADDR_USER))
//		{
//			//����������־
//			SetUpgrateResultAndFlag(UPGRATE_FLAG_VALUE,UPGRATE_INIT);
//		}
//	}
//	//��Ӵ�SD��copy�������ó���Flashָ��λ�ã��������Ƴ�
//	if(CheckIfCopyFactotyFileToFlash())
//	{
//		DriverLcdInit();
//	#if DEBUG_TRACE==1
//		printf("\r\nstart copy factory file from SD to Flash");
//	#endif
//		CopyFileFromSdToFlashDefault(FILE_START_ADDR_DEFAULT,FILE_LEN_ADDR_DEFAULT);
//	}
	
	//���ָ��������ó���Ҳû��������־
	if(!CheckIfRestoreFactoryKeyPressed())
	{
		if(!CheckIfNeedUpgrateFile())
		{
//			SetUpgrateResultAndFlag(UPGRATE_FLAG_CLEAR,UPGRATE_OK);
			JumpToRunUserCode();
		}			
	}
	//��Ҫ��ʼ��������	
	DriverCrcInit();
	DriverEepromInit();
	DriverLcdInit();

	if(CheckUpgrateFileCrc())
	{
		//����������ǳ������ó������ڿ�ʼ��ʱ������������־��Ŀ����Ҫ�Ƿ�ֹ���������� ��������У�����ϵ絼������ʧ��
		//���������ϵ���ܹ������û�����
		if(CheckIfUpgrateFactoryCode())
		{
			SetUpgrateResultAndFlag(UPGRATE_FLAG_VALUE,UPGRATE_INIT);
		}
		
		//����3��ʧ��
		UpgrateCount = 0;
		while(UpgrateCount < 3)
		{
			//��������ɹ������������᷵�ش˴���ֱ������ϵͳ����תӦ�ó���
			if(StartUpgrateProcess()==FALSE)
			{
				UpgrateCount++;
			}
		}
		//��������ĳ����ǳ������ó�����Ҫ�ָ��������ó���
		if(!CheckIfUpgrateFactoryCode())
		{
			gUpgrateFileLenAddr = FILE_LEN_ADDR_DEFAULT;
			gUpgrateFileStartAddr = FILE_START_ADDR_DEFAULT;
			if(CheckUpgrateFileCrc())
			{
				UpgrateCount = 0;
				while(UpgrateCount < 3)
				{
					//��������ɹ������������᷵�ش˴���ֱ������ϵͳ����תӦ�ó���
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






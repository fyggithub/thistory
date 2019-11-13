#ifndef __Boot_H__
#define __Boot_H__


#include "Datatypes.h"
#include "Gpio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "Bits.h"

typedef  void (*pFunction)(void);

#ifdef	STM32F103xE
#define FLASH_RUN_START_ADDR		(UINT32)0x08004000		//�������жε�ַ
#define FLASH_BKP_START_ADDR		(UINT32)0x08024000		//���������ε�ַ
#define FLASH_OFFSET				512                      //������ƫ��512���ֽڿ�ʼ
#define START_PAGE_NUM				9        				//�ӵ�9ҳ��ʼ����
#define PAGE_NUM					56       				//����56ҳ
#define PER_PAGE_ADDR_SPACE			2048	 				//ÿһҳ��ռ�Ŀռ�Ϊ2K		
#define LEN_OFFSET					32
#else	
#define FLASH_RUN_START_ADDR		(UINT32)0x08004000		//�������жε�ַ
#define FLASH_BKP_START_ADDR		(UINT32)0x08014000		//���������ε�ַ
#define FLASH_OFFSET				512                      //������ƫ��512���ֽڿ�ʼ
#define START_PAGE_NUM				9        				//�ӵ�9ҳ��ʼ����
#define PAGE_NUM					24       				//����24ҳ
#define PER_PAGE_ADDR_SPACE			1024	 				//ÿһҳ��ռ�Ŀռ�Ϊ1K		
#define LEN_OFFSET					32
#endif

VOID CRC32_INIT(VOID);
UINT16 WriteFlashWord(UINT32 addr,UINT32 data);
VOID BootLoadRun(VOID);











#endif





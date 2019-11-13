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
#define FLASH_RUN_START_ADDR		(UINT32)0x08004000		//代码运行段地址
#define FLASH_BKP_START_ADDR		(UINT32)0x08024000		//代码升级段地址
#define FLASH_OFFSET				512                      //运行区偏移512个字节开始
#define START_PAGE_NUM				9        				//从第9页开始擦除
#define PAGE_NUM					56       				//擦除56页
#define PER_PAGE_ADDR_SPACE			2048	 				//每一页所占的空间为2K		
#define LEN_OFFSET					32
#else	
#define FLASH_RUN_START_ADDR		(UINT32)0x08004000		//代码运行段地址
#define FLASH_BKP_START_ADDR		(UINT32)0x08014000		//代码升级段地址
#define FLASH_OFFSET				512                      //运行区偏移512个字节开始
#define START_PAGE_NUM				9        				//从第9页开始擦除
#define PAGE_NUM					24       				//擦除24页
#define PER_PAGE_ADDR_SPACE			1024	 				//每一页所占的空间为1K		
#define LEN_OFFSET					32
#endif

VOID CRC32_INIT(VOID);
UINT16 WriteFlashWord(UINT32 addr,UINT32 data);
VOID BootLoadRun(VOID);











#endif





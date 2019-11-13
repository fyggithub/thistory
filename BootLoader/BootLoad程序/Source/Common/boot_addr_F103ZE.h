/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * boot_addr_F103ZE.h
 *******************************************************************/
 
#ifndef _BOOT_ADDR_F103ZE_H_
#define _BOOT_ADDR_F103ZE_H_

#include "Datatypes.h"
/********************************************************************
 *
 *******************************************************************/

#define IAP_HEADER_FLASH_ADDR		  (0x08004000)
#define IAP_HEADER_FLASH_ADDR_ATTRIB  __attribute__((section(".ARM.__at_0x08004000")))

#define VERSION	(0x00010004)

/********************************************************************
 *
 *******************************************************************/
typedef struct _IAPHeader{
	UINT32 CRC0;	    // bin文件的CRC32校验码，Keil下载则为全FF
	UINT32 CRC1;	    // boot复制到运行区后重新校验，启动时如等于CRC0表示升级成功，可以启动
	UINT32 length;
	UINT32 version;
	UINT8 res[512-16];	// STM32芯片要求中断向量表必须512字节对齐
}IAPHeader;

#endif // _BOOT_ADDR_F103ZE_H_

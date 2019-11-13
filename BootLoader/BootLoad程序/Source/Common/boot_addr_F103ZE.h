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
	UINT32 CRC0;	    // bin�ļ���CRC32У���룬Keil������ΪȫFF
	UINT32 CRC1;	    // boot���Ƶ�������������У�飬����ʱ�����CRC0��ʾ�����ɹ�����������
	UINT32 length;
	UINT32 version;
	UINT8 res[512-16];	// STM32оƬҪ���ж����������512�ֽڶ���
}IAPHeader;

#endif // _BOOT_ADDR_F103ZE_H_

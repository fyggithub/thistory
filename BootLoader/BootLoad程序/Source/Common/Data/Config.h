/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Config.h
 *******************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"

#define CONFIG_MAGIC 	(0x21474643)
#define CONFIG_VERSION 	(1)
/********************************************************************
 *
 *******************************************************************/
typedef struct _ConfigDefault{
	INT32 defaultv;
	INT32 minv;
	INT32 maxv;
}ConfigDefault;

enum{
	CFG_CRC32 = 0,
	CFG_MAGIC,
	CFG_VERSION,
	CFG_TOTAL,
	CFG_HARDVER,
	CFG_MODUVER,
	CFG_USER_BEGIN = 16,
};
/********************************************************************
 *
 *******************************************************************/
VOID  ConfigInit(VOID);
BOOL  ConfigCheck(VOID);
VOID  ConfigSetDone(VOID);
VOID  ConfigResetDefault(VOID);
INT32 ConfigGet(UINT16 id);
INT32 ConfigSet(UINT16 id, INT32 v);
DWORD ConfigGetDword(UINT16 id);
DWORD ConfigSetDword(UINT16 id, DWORD dword);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _CONFIG_H_


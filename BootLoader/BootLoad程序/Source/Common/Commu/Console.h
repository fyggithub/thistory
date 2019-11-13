/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Console.h
 *******************************************************************/
#ifndef _CONSOLE_H_
#define _CONSOLE_H_
/********************************************************************
 * Console
 *******************************************************************/
#include "Datatypes.h"
#include "Board.h"
#include "Message.h"
/********************************************************************
 * Console Method
 *******************************************************************/
#ifdef EN_CONSOLE
VOID DebugLog     (UINT8 tag, CHAR* fmt, ...);
VOID DebugWarning (UINT8 tag, CHAR* fmt, ...);
VOID DebugError   (UINT8 tag, CHAR* fmt, ...);
VOID DebugHex     (UINT8 tag, BYTE* arr, UINT16 len);
VOID DebugHexColor(UINT8 tag, BYTE* arr, UINT16 len);
// tag 0 for system tag
UINT8 DebugRegister  (CHAR* str);
VOID  ConsoleInit    (VOID);
VOID  ConsoleProcess (VOID);
VOID  ConsoleRegister(CHAR* str, UINT32 msgId);
#else  // EN_CONSOLE
#define DebugLog(tag, fmt, ...)
#define DebugWarning(tag, fmt, ...)
#define DebugError(tag, fmt, ...)
#define DebugHex(tag, arr, len)
#define DebugHexColor(tag, arr, len)
#define DebugRegister(str)	(0)
#define ConsoleInit()
#define ConsoleProcess()
#define ConsoleRegister(str, msgId)
#endif // EN_CONSOLE
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _CONSOLE_H_

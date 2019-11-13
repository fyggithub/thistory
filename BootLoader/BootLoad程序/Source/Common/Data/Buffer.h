/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * 
 * Buffer.h
 *******************************************************************/
#ifndef _BUFFER_H_
#define _BUFFER_H_
/********************************************************************
 * Loop Buffer Struct
 *******************************************************************/
#include "Datatypes.h"

typedef struct __Buffer{
	BYTE*  array;	// 
	UINT16 size;	// size must > 1
	UINT16 idxr;	// read position
	UINT16 idxw;	// write position
}Buffer;
/********************************************************************
 * Loop Buffer Method
 *******************************************************************/
BOOL   BufferCreate (Buffer* buff, BYTE* arr, UINT16 size);
BOOL   BufferIsEmpty(Buffer* buff);
BOOL   BufferIsFull (Buffer* buff);
UINT16 BufferSize   (Buffer* buff);
UINT16 BufferLength (Buffer* buff);
UINT16 BufferPeek   (Buffer* buff, BYTE* dst, UINT16 len);
UINT16 BufferPop    (Buffer* buff, BYTE* dst, UINT16 len);
UINT16 BufferAppend (Buffer* buff, BYTE* src, UINT16 len);
UINT16 BufferAppendByte(Buffer* buff, BYTE byte);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BUFFER_H_

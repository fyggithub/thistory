/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * 
 * Buffer.c
 *******************************************************************/
#include <string.h>
#include "Buffer.h"
/********************************************************************
 * size must > 1
 *******************************************************************/
BOOL   BufferCreate (Buffer* buff, BYTE* arr, UINT16 size)
{
	if ( buff && arr && size > 1 ){
		buff->array = arr;
		buff->size  = size;
		buff->idxr  = buff->idxw = 0;
		return TRUE;
	}else{
		if (buff){
			buff->array = NULL;
			buff->size  = 0;
		}
	}
	return FALSE;
}
/********************************************************************
 * if buff is null, will return true
 *******************************************************************/
BOOL   BufferIsEmpty(Buffer* buff)
{
	if (buff){
		if ( buff->idxr != buff->idxw )
			return FALSE;
	}
	return TRUE;
}
/********************************************************************
 * if buff is null, will return true
 *******************************************************************/
BOOL   BufferIsFull (Buffer* buff)
{
	if (buff) {
		UINT16 idxw = buff->idxw;
		UINT16 nextw = (idxw+1) % (buff->size);
		if (nextw != buff->idxr)
			return FALSE;
	}
	return TRUE;
}
/********************************************************************
 * if buff is null, will return 0
 *******************************************************************/
UINT16 BufferSize   (Buffer* buff)
{
	if (buff) {
		return buff->size;
	}
	return 0;
}
/********************************************************************
 * if buff is null, will return 0
 *******************************************************************/
UINT16 BufferLength (Buffer* buff)
{
	if (buff) {
		UINT16 idxw = buff->idxw;
		UINT16 idxr = buff->idxr;
		UINT16 size = buff->size;
		if (idxw >= idxr)
			return (idxw - idxr);
		else
			return (idxw + size - idxr);
	}
	return 0;
}
/********************************************************************
 * copy but do not move idxr
 *******************************************************************/
UINT16 BufferPeek   (Buffer* buff, BYTE* dst, UINT16 len)
{
	if (buff) {
		// how many bytes to copy?
		UINT16 total = BufferLength(buff);
		len = total >= len ? len : total;
		// if just have 1 segment to copy
		UINT16 idxr = buff->idxr;
		UINT16 idxw = buff->idxw;
		BYTE*  arr  = buff->array;
		if ( idxr <= idxw ) {
			memcpy (dst, &(arr[idxr]), len );
		}
		// or have 2 segment to copy
		else{
			UINT16 seg1 = BufferSize(buff) - idxr;
			seg1 = seg1 >= len ? len : seg1;
			UINT16 seg2 = len - seg1;
			memcpy (dst, &(arr[idxr]), seg1);
			if (seg2)
				memcpy (dst+seg1, &(arr[0]), seg2);
		}
		return len;
	}
	return 0;
}
/********************************************************************
 * copy and move idxr
 *******************************************************************/
UINT16 BufferPop    (Buffer* buff, BYTE* dst, UINT16 len)
{
	len = BufferPeek ( buff, dst, len );
	if (len && buff) {
		UINT16 idxr = buff->idxr + len;
		buff->idxr = idxr % BufferSize(buff);
	}
	return len;
}
/********************************************************************
 * append byte array
 *******************************************************************/
UINT16 BufferAppend (Buffer* buff, BYTE* src, UINT16 len)
{
	if (buff) {
		// how many free byte?
		UINT16 freebyte = BufferSize(buff) - BufferLength(buff) - 1;
		// reset copy length
		len = len > freebyte ? freebyte : len;
		// if just have 1 segment free room for copy
		UINT16 idxr = buff->idxr;
		UINT16 idxw = buff->idxw;
		BYTE*  arr  = buff->array;
		if ( idxw < idxr ) {
			memcpy ( &arr[idxw], src, len );
			buff->idxw += len;
		}
		// or have 2 segment for copy
		else{
			UINT16 seg1 = BufferSize(buff) - idxw;
			seg1 = seg1 >= len ? len : seg1;
			UINT16 seg2 = len - seg1;
			memcpy ( &arr[idxw] , src, seg1 );
			
			if (seg2){
				memcpy ( &arr[0], src+seg1, seg2 );
				buff->idxw = seg2;
			}else {
				buff->idxw += len;
			}
		}
		return len;

	}
	return 0;
}
/********************************************************************
 * append one byte
 *******************************************************************/
UINT16 BufferAppendByte(Buffer* buff, BYTE byte)
{
	if (buff && !BufferIsFull(buff) ){
		BYTE* arr = buff->array;
		arr [ buff->idxw++ ] = byte;
		buff->idxw %= buff->size;
		return 1;
	}
	return 0;
}
/********************************************************************
 * END OF FILE
 *******************************************************************/
 

/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Console.c
 *******************************************************************/
#include "Console.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef EN_CONSOLE
/********************************************************************
 *
 *******************************************************************/
#define DEBUG_MSG_LEN	(128)
#define DEBUG_TAG_LEN	(4)
#define DEBUG_TAG_MAX	(16)

#define CONSOLE_RX_LEN	(64)
#define CONSOLE_TX_LEN	DEBUG_MSG_LEN
#define CONSOLE_CMD_LEN	(8)
#define CONSOLE_CMD_MAX	(16)
/********************************************************************
 *
 *******************************************************************/
typedef struct __DebugTagNode{
	CHAR tagName[DEBUG_TAG_LEN];
}DebugTagNode;

typedef struct __Console{
	Uart*   uart;
	CHAR    debugMsgBuf[DEBUG_MSG_LEN];
	BYTE	rxBufferArray[CONSOLE_RX_LEN];
	Buffer  rxBuffer;
	BYTE	txArray 	[CONSOLE_TX_LEN];
	UINT16  tagNum;
	DebugTagNode tagNode[DEBUG_TAG_MAX];
}Console;

typedef struct __ConsoleCmd{
	CHAR   str[CONSOLE_CMD_LEN+1];
	UINT32 msgId;
}ConsoleCmd;

typedef struct __ConsoleLine{
	BYTE    lineArray[CONSOLE_RX_LEN];
	UINT16  lineArrayLength;
	BYTE    lineBufferArray[CONSOLE_RX_LEN];
	Buffer  lineBuffer;
	Message lastMsg;
	UINT16     cmdNum;
	ConsoleCmd cmds[CONSOLE_CMD_MAX];
}ConsoleLine;

STATIC Console console;
STATIC ConsoleLine consoleLine;

#define MsgBufEmpty()	do{ console.debugMsgBuf[0] =0; }while(0)
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MsgBufSend(VOID)
{
	UINT16 len = (UINT16)strlen(console.debugMsgBuf);
	UartSend(console.uart, (BYTE*)(console.debugMsgBuf), len);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MsgBufAppendTag(UINT8 tag)
{
	if (tag < console.tagNum) {
		UINT16 len = (UINT16) strlen(console.debugMsgBuf);
		sprintf (console.debugMsgBuf+len, "%s> ",
			console.tagNode[tag].tagName);
	}
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MsgBufAppend(CHAR* fmt, va_list params)
{
	UINT16 len = (UINT16) strlen(console.debugMsgBuf);
	vsprintf(console.debugMsgBuf+len, fmt, params);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MsgBufAppendCRLF(VOID)
{
	UINT16 len = (UINT16) strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+len, "\r\n");
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID MsgBufHex(UINT8 tag, BYTE* arr, UINT16 len)
{
	MsgBufAppendTag(tag);
	UINT16 bufLen = (UINT16) strlen(console.debugMsgBuf);

	for (UINT16 i=0; i<len; i++) {
		sprintf (console.debugMsgBuf+bufLen, "%02X ", arr[i] );
		bufLen += 3;
	}
	sprintf (console.debugMsgBuf+bufLen, "\r\n");
}
/********************************************************************
 *
 *******************************************************************/
VOID DebugLog     (UINT8 tag, CHAR* fmt, ...)
{
	MsgBufEmpty();

	MsgBufAppendTag(tag);
	va_list params;
	va_start (params, fmt);
	MsgBufAppend (fmt, params);
	va_end (params);

	MsgBufAppendCRLF();
	MsgBufSend();
}
/********************************************************************
 *
 *******************************************************************/
VOID DebugWarning (UINT8 tag, CHAR* fmt, ...)
{
	MsgBufEmpty();
	UINT16 len = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+len, "\033[1;33m");

	MsgBufAppendTag(tag);
	va_list params;
	va_start (params, fmt);
	MsgBufAppend (fmt, params);
	va_end (params);

	len = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+len, "\033[1;39m");

	MsgBufAppendCRLF();
	MsgBufSend();
}
/********************************************************************
 *
 *******************************************************************/
VOID DebugError   (UINT8 tag, CHAR* fmt, ...)
{
	MsgBufEmpty();
	UINT16 len = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+len, "\033[1;31m");

	MsgBufAppendTag(tag);
	va_list params;
	va_start (params, fmt);
	MsgBufAppend (fmt, params);
	va_end (params);

	len = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+len, "\033[1;39m");

	MsgBufAppendCRLF();
	MsgBufSend();
}
/********************************************************************
 *
 *******************************************************************/
VOID DebugHex     (UINT8 tag, BYTE* arr, UINT16 len)
{
	MsgBufEmpty();
	UINT16 bufLen = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+bufLen, "\033[0m");

	MsgBufHex (tag, arr, len);

	bufLen = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+bufLen, "\033[1m");
	MsgBufSend();
}
/********************************************************************
 *
 *******************************************************************/
VOID DebugHexColor(UINT8 tag, BYTE* arr, UINT16 len)
{
	MsgBufEmpty();
	UINT16 bufLen = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+bufLen, "\033[0;36m");

	MsgBufHex (tag, arr, len);

	bufLen = (UINT16)strlen(console.debugMsgBuf);
	sprintf(console.debugMsgBuf+bufLen, "\033[1;39m");
	MsgBufSend();
}
/********************************************************************
 *
 *******************************************************************/
UINT8 DebugRegister  (CHAR* str)
{
	if ( str ) {
		UINT8 i = console.tagNum;
		if ( i < DEBUG_TAG_MAX ) {
			strncpy (console.tagNode[i].tagName, str, DEBUG_TAG_LEN);
			console.tagNode[i].tagName[DEBUG_TAG_LEN-1] = 0;
			console.tagNum ++;
			return i;
		} else {
			DebugError (0, "Debug Tag Register Error - Full!");
		}
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT16 TransCR2CRLF(UINT8* buf, UINT16 len)
{
	UINT16 i=0;
	while(i<len){
		if(buf[i] == 0x0d){
			memmove(&buf[i+2], &buf[i+1], len-i);
			buf[i+1] = 0x0a;
			i++;
			len++;
		}
		i++;
	}
	return len;
}
/********************************************************************
 *
 *******************************************************************/
VOID  ConsoleInit(VOID)
{
	// Init Debug Manager
	console.uart = BoardGetConsoleUart();
	BufferCreate ( &(console.rxBuffer),
		console.rxBufferArray, CONSOLE_RX_LEN );
	UartInit(console.uart, &(console.rxBuffer),
		console.txArray, CONSOLE_TX_LEN);
	console.tagNum = 0;

	// Init Console Manager
	BufferCreate ( &(consoleLine.lineBuffer),
		consoleLine.lineBufferArray, CONSOLE_RX_LEN);
	consoleLine.cmdNum = 0;
	consoleLine.lastMsg.id = MSG_UNKNOWN;

	// Register system tag
	DebugRegister("sys"); // 0 for system
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL CheckConsoleLineCR(VOID)
{
	UINT16 len = BufferPeek (&consoleLine.lineBuffer,
		consoleLine.lineArray, CONSOLE_RX_LEN);
	if ( len == 0) return FALSE;

	UINT16 i = 0;
	BYTE* arr = consoleLine.lineArray;
	for(; i<len; i++){
		if (arr[i] == 0x0d) break;
	}
	if ( i==len ) return FALSE;

	BufferPop(&consoleLine.lineBuffer, arr, i+1); // include 0x0d
	consoleLine.lineArrayLength = i+1;
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID RemoveBackspaceInLineArray(VOID)
{
	UINT16 i = 0;
	BYTE* arr = consoleLine.lineArray;
	UINT16 total = consoleLine.lineArrayLength;
	while (i<total){
		if ( arr[i] == 0x08 ){
			if (i) {
				memmove( &(arr[i-1]), &(arr[i+1]), total-(i+1));
				total -= 2;
			} else {
				memmove( arr, &(arr[1]), total-1);
				total -= 1;
			}
		} else {
			i++;
		}
	}
	consoleLine.lineArrayLength = total;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL
SearchNextWordInLineArray(CHAR* word, UINT16 wordLen, UINT16* beg)
{
	if( word && beg && wordLen > 0) {
		UINT16 i = *beg;
		BYTE* buff = consoleLine.lineArray;
		UINT16 len = consoleLine.lineArrayLength;

		while ( i<len && (buff[i]==0x20 || buff[i] == '\t')) i++;
		if ( i==len || buff[i] == 0x0d || buff[i] == 0 )
			return FALSE;
		UINT16 j = i;

		while ( j<len &&
					(buff[j]!=0x20 && buff[j] != '\t' &&
					 buff[j]!=0x0d && buff[j] != 0)) {
			j++;
		}
		if (i != j) {
			len = j-i;
			len = len > wordLen-1 ? wordLen-1 : len;
			memcpy ( word, &(buff[i]), len);
			word[len] = 0; // cut off
			*beg = j+1; // iter
			return TRUE;
		}
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL SearchConsoleCommand(CHAR* word, UINT32* msgId)
{
	if ( word && msgId ) {
		UINT16 num = consoleLine.cmdNum;
		for (UINT16 i=0; i<num; i++){
			if (!strcmp(word, consoleLine.cmds[i].str)){
				*msgId = consoleLine.cmds[i].msgId;
				return TRUE;
			}
		}
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL IsRepeatLastMessage(CHAR* word)
{
	if (word) {
		if (!strcmp(word, "=")) return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
#define LINE_WORD_SIZE	(20)
#define LINE_PARAMS_MAX (4)
STATIC VOID ConsoleLineParser(VOID)
{
	CHAR word[LINE_WORD_SIZE];
	BOOL isLineFinished = CheckConsoleLineCR();
	if (isLineFinished) {
		UINT16 beg, pi;
		UINT32 msgId;
		INT32  params[LINE_PARAMS_MAX] = {0, 0, 0, 0};
		beg = pi = 0;
		RemoveBackspaceInLineArray();

		if (SearchNextWordInLineArray (
				word, LINE_WORD_SIZE, &beg)) {
			if ( !IsRepeatLastMessage(word)) {
				if (SearchConsoleCommand (word, &msgId) ) {
					while (SearchNextWordInLineArray (
							word, LINE_WORD_SIZE, &beg)){
						params[pi++] = atoi(word);
						if (pi == LINE_PARAMS_MAX) break;
					}
					consoleLine.lastMsg.id = msgId;
					consoleLine.lastMsg.p1 = params[0];
					consoleLine.lastMsg.p2 = params[1];
					consoleLine.lastMsg.p3 = params[2];
					consoleLine.lastMsg.p4 = params[3];
					MessagePost(& (consoleLine.lastMsg) );
				}
			}else
				MessagePost(& (consoleLine.lastMsg) );
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID  ConsoleProcess (VOID)
{
	UINT16 len = BufferLength (&console.rxBuffer);
	if (len) {
		len = BufferPop(&console.rxBuffer, consoleLine.lineArray, len);
		BufferAppend(&consoleLine.lineBuffer, consoleLine.lineArray, len);
		// Console Echo
		len = TransCR2CRLF (consoleLine.lineArray, len);
		UartSend (console.uart, consoleLine.lineArray, len);
		// Console Line Parser
		ConsoleLineParser();
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID  ConsoleRegister(CHAR* str, UINT32 msgId)
{
	if (str && msgId != MSG_UNKNOWN) {
		UINT16 i = consoleLine.cmdNum;
		if (i<CONSOLE_CMD_MAX) {
			consoleLine.cmds[i].msgId = msgId;
			strncpy (consoleLine.cmds[i].str, str, CONSOLE_CMD_LEN);
			consoleLine.cmds[i].str[CONSOLE_CMD_LEN] = 0; // cut off
			consoleLine.cmdNum ++;
			DebugLog (0, "Console Register - %s => %d", str, msgId);
		}else {
			DebugError(0, "Console Register Error - Full!");
		}
	}
}
/********************************************************************
 * End of File
 *******************************************************************/
#endif


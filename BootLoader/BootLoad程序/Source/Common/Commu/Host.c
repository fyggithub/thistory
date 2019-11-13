/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Host.c
 *******************************************************************/
#include "Host.h"
#include "Console.h"
#include "Uart.h"
#include "Board.h"
#include "Buffer.h"
#include "Bits.h"
#include <string.h>
/********************************************************************
 *
 *******************************************************************/
#define HOST_IDENTIFY	 (0xE1C3)
#define HOST_ADDR		 (0x01)
#define HOST_TYPE_ASK	 (0x01)
#define HOST_TYPE_ACK	 (0x11)
#define HOST_TYPE_RESULT (0x12)
/********************************************************************
 *
 *******************************************************************/
#define HOST_RX_LEN		(0x64)
#define HOST_TX_LEN		(0x64)
#define HOST_PARSER_MAX	(2)

typedef struct __HostParser{
	UINT16 addr;
	PARSER parser;
}HostParser;

typedef struct __Host{
	Uart*  uart;
	BYTE   txArray[HOST_TX_LEN];
	//BYTE   rxArray[HOST_RX_LEN];
	BYTE   rxBufferArray[HOST_RX_LEN];
	Buffer rxBuffer;
	HostParser parsers[HOST_PARSER_MAX];
	UINT16 parserNum;
	HostFrame lastFrame;
	BOOL isOpenLog;
}Host;
STATIC Host host;
STATIC UINT8 tag;
/********************************************************************
 *
 *******************************************************************/
STATIC UINT16 CalcCrc16(BYTE* buff, UINT16 len)
{
	CONST UINT16 CRC_QUANTIC = 0xA001;
	UINT16 crc = 0;
	for(UINT16 i = 0; i<len; i++){
		crc = crc^(*buff);
		buff++;
		for(UINT8 j=0; j<8; j++){
			if((crc&0x0001) == 1){
				crc = crc >> 1;
				crc = crc ^ CRC_QUANTIC;
			}else{
				crc = crc >> 1;
			}
		}
	}
	return crc;
}
/********************************************************************
 *
 *******************************************************************/
VOID SetHostOpenLogPra(BOOL w)
{
	host.isOpenLog = w;
}
/********************************************************************
 *
 *******************************************************************/
VOID HostAck     (UINT32 ack)
{
	BYTE ackBuf[13];
	ackBuf[0] = HBYTE (HOST_IDENTIFY);
	ackBuf[1] = LBYTE (HOST_IDENTIFY);
	ackBuf[2] = 0;
	ackBuf[3] = 7;
	ackBuf[4] = HBYTE (host.lastFrame.number);
	ackBuf[5] = LBYTE (host.lastFrame.number);
	ackBuf[6] = HOST_ADDR;
	ackBuf[7] = HOST_TYPE_ACK;
	ackBuf[8] = host.lastFrame.command;
	ackBuf[9] = host.lastFrame.sub;
	ackBuf[10] = ack;
	UINT16 crc = CalcCrc16 (ackBuf+4, 7);
	ackBuf[11] = HBYTE (crc);
	ackBuf[12] = LBYTE (crc);
	UartSend (host.uart, ackBuf, 13);
	if(host.isOpenLog == FALSE)
		return;
	DebugHex (tag, ackBuf, 13);
}
/********************************************************************
 *
 *******************************************************************/
VOID HostResponse(BYTE* data, UINT16 len)
{
	UINT16 frameLen = len+6;
	BYTE respBuf[HOST_TX_LEN];
	respBuf[0] = HBYTE (HOST_IDENTIFY);
	respBuf[1] = LBYTE (HOST_IDENTIFY);
	respBuf[2] = HBYTE (frameLen);
	respBuf[3] = LBYTE (frameLen);
	respBuf[4] = HBYTE (host.lastFrame.number);
	respBuf[5] = LBYTE (host.lastFrame.number);
	respBuf[6] = HOST_ADDR;
	respBuf[7] = HOST_TYPE_RESULT;
	respBuf[8] = host.lastFrame.command;
	respBuf[9] = host.lastFrame.sub;
	memcpy (respBuf+10, data, len);
	UINT16 crc = CalcCrc16 (respBuf+4, frameLen);
	respBuf[frameLen+4] = HBYTE (crc);
	respBuf[frameLen+5] = LBYTE (crc);
	UartSend (host.uart, respBuf, frameLen+6);
	if(host.isOpenLog == FALSE)
		return;
	DebugHex (tag, respBuf, frameLen+6);
}
/********************************************************************
 *
 *******************************************************************/
VOID HostSend(BYTE* frame, UINT16 len)
{
	UINT16 frameLen = len+6;
    UINT16 crc = CalcCrc16 (frame+4, frameLen);
	frame[frameLen+4] = HBYTE (crc);
	frame[frameLen+5] = LBYTE (crc);
	UartSend (host.uart, frame,  frameLen+6);
	if(host.isOpenLog == FALSE)
		return;
	DebugHex (tag, frame,  frameLen+6);
}

/********************************************************************
 *
 *******************************************************************/
VOID HostInit    (VOID)
{
	tag = DebugRegister ("hst");
	host.uart = BoardGetHostUart();
	BufferCreate (&(host.rxBuffer), host.rxBufferArray, HOST_RX_LEN);
	UartInit (host.uart, &(host.rxBuffer), host.txArray, HOST_RX_LEN);
	host.parserNum = 0;
	host.lastFrame.number = (UINT16)(-1);
	host.isOpenLog = TRUE;
	DebugLog(tag, "Host Init Finished.");
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCheckCRC(BYTE* rxBuf, UINT16 rxLen, UINT16 frameLen)
{
	UINT16 crcRecv = MAKE_WORD (rxBuf[rxLen-2], rxBuf[rxLen-1]);
	UINT16 crcCalc = CalcCrc16 (&rxBuf[4], frameLen);
	if (crcRecv == crcCalc)
		return TRUE;
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC INT16 HostSearchParser(UINT8 addr)
{
	UINT16 i=0;
	for (; i<HOST_PARSER_MAX; i++) {
		if (host.parsers[i].addr == (UINT16)addr) break;
	}
	if (i == HOST_PARSER_MAX) return -1;
	return i;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID HostFrameParser(UINT16 len)
{
	if ( len > 10 ){
		HostFrame* frame = &(host.lastFrame);
		BYTE* data = frame->data;
		UINT16 frameLen = MAKE_WORD (data[2], data[3]);
		frame->number = MAKE_WORD (data[4], data[5]);
		UINT8  addr   = data[6];
		frame->type   = data[7];
		frame->command= data[8];
		frame->sub    = data[9];
		
		if(host.isOpenLog == TRUE)
			DebugHexColor (tag, data, len);
		if (!HostCheckCRC (data, len, frameLen)){
			HostAck (HOST_ACK_ERR_CRC);
			DebugWarning(tag, "Found CRC Error!");
			return;
		}
		INT16 iParser = HostSearchParser (addr);
		if (iParser == -1){
			HostAck (HOST_ACK_ERR_ADDR);
			DebugWarning (tag, "Found Address Error!");
			return;
		}

		UINT32 ack;
		memmove(data, data+10, frameLen-4);
		frame->dataLen = frameLen-4;
		if ( host.parsers[iParser].parser ){
			ack = host.parsers[iParser].parser(&(host.lastFrame));
			HostAck(ack);
		}
	} else {
		DebugError(tag, "Frame is too short?");
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID HostProcess (VOID)
{
	UINT16 len = BufferLength (&(host.rxBuffer));
	if (len >= 4) {
		BYTE header[4];
		BufferPeek (&(host.rxBuffer), header, 4);
		UINT16 identify = MAKE_WORD (header[0], header[1]);
		if (identify == HOST_IDENTIFY) {
			UINT16 frameLen = MAKE_WORD (header[2], header[3]);
			if (len >= frameLen+6 ){
				len = BufferPop (&(host.rxBuffer),
					host.lastFrame.data, frameLen+6);
				if (len == frameLen+6){
					HostFrameParser (len);
				} else {
					DebugError(tag, "What's Wrong?");
				}
			}
		} else {
			// throw 1 byte
			BufferPop(&(host.rxBuffer), header, 1);
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID HostParserRegister (UINT16 addr, PARSER parser)
{
	if (parser){
		if (host.parserNum < HOST_PARSER_MAX){
			UINT16 i = host.parserNum;
			host.parsers[i].addr   = addr;
			host.parsers[i].parser = parser;
			host.parserNum ++;
			DebugLog (tag, "Host Parser Register - addr(%02X)", addr);
		} else {
			DebugError (tag, "Host Parser Register Error - Full!");
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT8 HostGetConsoleTag(VOID)
{
	return tag;
}
/********************************************************************
 * END OF FILE
 *******************************************************************/



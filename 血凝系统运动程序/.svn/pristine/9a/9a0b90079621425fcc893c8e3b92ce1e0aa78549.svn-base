/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Host.h
 *******************************************************************/
#ifndef _HOST_H_
#define _HOST_H_
/********************************************************************
 * Host defined
 *******************************************************************/
#include "Datatypes.h"

#define HOST_ACK_SUCCESS	(0x00)
#define HOST_ACK_ERR_LEN	(0x01)
#define HOST_ACK_ERR_CRC	(0x02)
#define HOST_ACK_ERR_ADDR	(0x03)

#define HOST_FRAME_DATALEN	(64)


/********************************************************************
 *
 *******************************************************************/
typedef struct __HostFrame{
	UINT16 number;
	UINT8  type;
	UINT8  command;
	UINT8  sub;
	BYTE   data[HOST_FRAME_DATALEN];
	UINT16 dataLen;
}HostFrame;
typedef UINT32 (*PARSER)(HostFrame* frame); // return ack
/********************************************************************
 * Host Method
 *******************************************************************/
VOID HostAck     (UINT32 ack);
VOID HostResponse(BYTE* data, UINT16 len);
VOID HostSend(BYTE* frame, UINT16 len);
VOID HostFrameSteal(BYTE* data);
VOID HostInit    (VOID);
VOID HostProcess (VOID);
VOID HostParserRegister (UINT16 addr, PARSER parser);
UINT8 HostGetConsoleTag(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _HOST_H_

/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostHub.c
 *******************************************************************/
#include "HostHub.h"
#include "Host.h"
#include "Console.h"
#include "Message.h"
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdVersion(HostFrame* frame)
{
	Message msg;	
	msg.id = MSG_HOST_VERSION;
	MessagePost(&msg);
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdGetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdSetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdParaInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdLedOn(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_LED_ON;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdLedOff(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_LED_OFF;
		MessagePost(&msg);
		return TRUE;
	}
    return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdPdDetectOn(HostFrame* frame)
{
    Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_PD_DETECT_ON;
        msg.p1 = frame->data[2];
        msg.p2 = frame->data[0];
		MessagePost(&msg);
		return TRUE;
	}
    return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdPdDetectOff(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_PD_DETECT_OFF;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdPdData(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_PD_DATA;
        msg.p1 = (frame->data[0]<<16)+(frame->data[1]<<8) + frame->data[2];
        msg.p2 = frame->data[3];
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdPdTime(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_PD_TIME;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMagStart  (HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_MAG_START;
        msg.p1 = frame->data[0];
        msg.p2 = frame->data[1];
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMagStop  (HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_MAG_STOP;
        msg.p1 = frame->data[0];
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMagRead  (HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_MAG_READ;
        msg.p1 = (frame->data[0]<<16)+(frame->data[1]<<8)+frame->data[2];
        msg.p2 = frame->data[3];
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostPdMainParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	switch(frame->command){
	case HOST_CMD_VERSION       : ret = HostCmdVersion   (frame); break;
	case HOST_CMD_GETINFO       : ret = HostCmdGetInfo   (frame); break;
	case HOST_CMD_SETINFO       : ret = HostCmdSetInfo   (frame); break;
	case HOST_CMD_PARAINFO      : ret = HostCmdParaInfo  (frame); break;
	case HOST_CMD_LED_ON	   	: ret = HostCmdLedOn     (frame); break;
	case HOST_CMD_LED_OFF		: ret = HostCmdLedOff    (frame); break;
	case HOST_CMD_PD_DETECT_ON	: ret = HostCmdPdDetectOn(frame); break;
	case HOST_CMD_PD_DETECT_OFF	: ret = HostCmdPdDetectOff(frame);break;
	case HOST_CMD_PD_DATA		: ret = HostCmdPdData    (frame); break;
	case HOST_CMD_PD_TIME		: ret = HostCmdPdTime    (frame); break;
    case HOST_CMD_MAG_START     : ret = HostCmdMagStart  (frame); break;
    case HOST_CMD_MAG_STOP      : ret = HostCmdMagStop   (frame); break;
    case HOST_CMD_MAG_READ      : ret = HostCmdMagRead   (frame); break;
	default:
		DebugWarning (tag, "Found Unknown Command!");
		return (HOST_ACK_ERR_ADDR);;
	}
	if(ret){
		return (HOST_ACK_SUCCESS);
	}else{
		DebugWarning(tag, "Found Length Error!");
		return (HOST_ACK_ERR_LEN);
	}
}


/********************************************************************
 *
 *******************************************************************/
VOID HostPdMainInit(VOID)
{
	HostParserRegister (HOST_ADDR_PDMAIN, HostPdMainParser);
}
/********************************************************************
 * End of File
 *******************************************************************/




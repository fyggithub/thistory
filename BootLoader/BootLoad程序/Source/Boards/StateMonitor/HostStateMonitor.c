/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostStateMonitor.c
 *******************************************************************/
#include "HostStateMonitor.h"
#include "Host.h"
#include "Console.h"
#include "Message.h"
#include "SerialDownLoad.h"
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdVersion(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_HOST_VERSION;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
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
STATIC BOOL HostCmdUpDateAsk(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT32 len = (((UINT32)frame->data[0])<<24) + (((UINT32)frame->data[1])<<16) + 
			(((UINT32)frame->data[2])<<8) + frame->data[3];
		SerialLoadInit(len);
		msg.id = MSG_HOST_UPDATEASK;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
*
*******************************************************************/
STATIC BOOL HostCmdUpDateTran(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		GetFileData(frame->data);
		msg.id = MSG_HOST_UPTRAN;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
*
*******************************************************************/
STATIC BOOL HostCmdStateAsk(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_HOST_STATE_ASK;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdStateLedCtr(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_HOST_STATE_LED_CONTROL;
		msg.p1 = frame->data[0];
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdEmergencyStop(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_HOST_EMERGENCY_STOP;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/

STATIC UINT32 HostStateMonitorParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	switch(frame->command){
	case HOST_CMD_VERSION       :	ret = HostCmdVersion (frame);     break;
	case HOST_CMD_GETINFO       :	ret = HostCmdGetInfo (frame);     break;
	case HOST_CMD_SETINFO       :	ret = HostCmdSetInfo (frame);     break;
	case HOST_CMD_PARAINFO      :	ret = HostCmdParaInfo (frame);    break;
	case HOST_CMD_STATE_ASK	    :	ret = HostCmdStateAsk (frame);       break;
	case HOST_CMD_STATE_LED_CONTROL	:	ret = HostCmdStateLedCtr (frame);       break;
	case HOST_CMD_EMERGENCY_STOP	:	ret = HostCmdEmergencyStop(frame);       break;
	case HOST_CMD_UPDATE_TRAN	:	ret = HostCmdUpDateTran(frame); break;
	case HOST_CMD_UPDATE_ASK	:	ret = HostCmdUpDateAsk(frame); break;		
	default:
		DebugWarning (tag, "Found Unknown Command!");
		return (HOST_ACK_ERR_ADDR);
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
VOID HostStateMonitorInit(VOID)
{
	HostParserRegister (HOST_ADDR_STATEMONITOR, HostStateMonitorParser);
}
/********************************************************************
 * End of File
 *******************************************************************/




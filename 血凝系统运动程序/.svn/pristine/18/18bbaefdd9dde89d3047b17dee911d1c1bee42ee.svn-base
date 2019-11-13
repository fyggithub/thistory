/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostHeat.c
 *******************************************************************/

#include "Console.h" 
#include "Message.h"
#include "gpio.h"
#include "Uart.h"
#include "Buffer.h"
#include "Board.h"
#include "Cool.h"
#include "HostCool.h"
#include "Host.h"

STATIC UINT8 tag;
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
STATIC BOOL  HostCmdCoolEnable (HostFrame* frame)
{
    Message msg;
    msg.id = MSG_ENABLE_COOL;
    msg.p1 = frame->data[0];
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL  HostCmdTemSet (HostFrame* frame)
{
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdTemAsk  (HostFrame* frame)
{
    Message msg;
    msg.id = MSG_ASK_TEMPERATURE;
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostCoolParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	switch(frame->command){
	case HOST_CMD_VERSION            : ret = HostCmdVersion    (frame);     break;
	case HOST_CMD_GETINFO            : ret = HostCmdGetInfo    (frame);     break;
	case HOST_CMD_SETINFO            : ret = HostCmdSetInfo    (frame);     break;
	case HOST_CMD_PARAINFO           : ret = HostCmdParaInfo   (frame);     break;
	case HOST_CMD_TEMPERATURE_ENABLE : ret = HostCmdCoolEnable (frame);     break;
	case HOST_CMD_TEMPERATURE_SET    : ret = HostCmdTemSet     (frame);     break;
    case HOST_CMD_TEMPERATURE_ASK    : ret = HostCmdTemAsk     (frame);     break;
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
VOID HostCoolInit(VOID)
{
    HostParserRegister (HOST_ADDR_COOL, HostCoolParser); 
    DebugLog(tag, "Host Init Finished.");
}
/********************************************************************
* END
*******************************************************************/

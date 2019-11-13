/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostLiquid.c
 *******************************************************************/

#include "Console.h" 
#include "Message.h"
#include "gpio.h"
#include "Uart.h"
#include "Buffer.h"
#include "Board.h"
#include "Liquid.h"
#include "HostLiquid.h"
#include "Host.h"

STATIC UINT8 tag[6];
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdVersion(HostFrame* frame)
{
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
STATIC BOOL  HostCmdParaSet (HostFrame* frame)
{
    Message msg;
    msg.id = MSG_NTC_RUN;
    UINT8 tmp0 = frame->data[0];
    UINT8 tmp1 = frame->data[1]; 
    msg.p1 = MAKE_WORD(tmp0,tmp1);
    tmp0 = frame->data[2];
    tmp1 = frame->data[3]; 
    msg.p2 = (DOUBLE)MAKE_WORD(tmp0,tmp1);
    tmp0 = frame->data[4];
    tmp1 = frame->data[5]; 
    msg.p3 = (DOUBLE)MAKE_WORD(tmp0,tmp1);
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL  HostCmdPwmSwitch (HostFrame* frame)
{
    Message msg;
    if(frame->data[0] == 0){
        msg.id = MSG_NTC_STOP;
        MessagePost(&msg);
    }
    if(frame->data[0] != 0){
        msg.id = MSG_NTC_READY;
        MessagePost(&msg);
    }
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdTemAsk  (HostFrame* frame)
{
    Message msg;
    msg.id = MSG_NTC_TEMPERATURE;
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostLiquidParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	switch(frame->command){
	case HOST_CMD_VERSION           : ret = HostCmdVersion (frame);     break;
	case HOST_CMD_GETINFO           : ret = HostCmdGetInfo (frame);     break;
	case HOST_CMD_SETINFO           : ret = HostCmdSetInfo (frame);     break;
	case HOST_CMD_PARAINFO          : ret = HostCmdParaInfo (frame);    break;
	case HOST_CMD_PARAMETER_SET	    : ret = HostCmdParaSet (frame);     break;
	case HOST_CMD_TEMPERATURE_ASK	: ret = HostCmdTemAsk  (frame);     break;
    case HOST_CMD_PWM_SWITCH        : ret = HostCmdPwmSwitch(frame);    break;
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
VOID HostLiquidInit(VOID)
{
    tag[5] = DebugRegister ("hst");
    tag[0] = DebugRegister ("CRY");
    tag[1] = DebugRegister ("CRN");
    tag[2] = DebugRegister ("STP");
    tag[3] = DebugRegister ("RRY");
    tag[4] = DebugRegister ("RRN");
    HostParserRegister (HOST_ADDR_LIQUID, HostLiquidParser); 
    DebugLog(tag[5], "Host Init Finished.");
}
/********************************************************************
* END
*******************************************************************/

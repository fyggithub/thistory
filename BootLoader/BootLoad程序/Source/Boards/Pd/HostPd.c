/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostLoad.c
 *******************************************************************/


#include "HostPd.h"
#include "Console.h"
#include "Message.h"
#include "BoardHost.h"
STATIC UINT8 tag;
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdVersion(VOID)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdGetInfo(VOID)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdSetInfo(VOID)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdParaInfo(VOID)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdLedOn(VOID)
{
    
	Message msg;
    msg.id = MSG_HOST_LED_ON;
    MessagePost(&msg);
    return TRUE;
}

/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdLedOff(VOID)
{
	Message msg;
    msg.id = MSG_HOST_LED_OFF;
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdPdDetectOn(VOID)
{
    Message msg;
    msg.id = MSG_HOST_PD_DETECT;
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostPdParser(BYTE cmd)
{
	BOOL ret = FALSE;
	switch(cmd){
        case HOST_CMD_VERSION       : ret = HostCmdVersion   	();	break;
        case HOST_CMD_GETINFO       : ret = HostCmdGetInfo   	();	break;
        case HOST_CMD_SETINFO       : ret = HostCmdSetInfo   	();	break;
        case HOST_CMD_PARAINFO      : ret = HostCmdParaInfo  	();	break;
        case HOST_CMD_LED_ON        : ret = HostCmdLedOn     	();	break;
        case HOST_CMD_LED_OFF       : ret = HostCmdLedOff    	();	break;
        case HOST_CMD_PD_DETECT     : ret = HostCmdPdDetectOn	();	break;
        default:
            DebugWarning (tag, "Found Unknown Command!");
        return (ret);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID HostPdInit(VOID)
{
    ObjCmdParserRegister(HostPdParser);
}
/********************************************************************
 * End of File
 *******************************************************************/




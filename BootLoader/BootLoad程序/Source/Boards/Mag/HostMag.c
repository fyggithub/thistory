/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostMag.c
 *******************************************************************/


#include "HostMag.h"
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
STATIC BOOL HostCmdMagDetectOn(VOID)
{
    Message msg;
    msg.id = MSG_HOST_MAG_DATA;
    MessagePost(&msg);
    return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostMagParser(BYTE cmd)
{
	BOOL ret = FALSE;
	switch(cmd){
        case HOST_CMD_VERSION       : ret = HostCmdVersion   	();	break;
        case HOST_CMD_GETINFO       : ret = HostCmdGetInfo   	();	break;
        case HOST_CMD_SETINFO       : ret = HostCmdSetInfo   	();	break;
        case HOST_CMD_PARAINFO      : ret = HostCmdParaInfo  	();	break;
        case HOST_CMD_MAG_DETECT    : ret = HostCmdMagDetectOn	();	break;
        default:
            DebugWarning (tag, "Found Unknown Command!");
        return (ret);
	}
	return ret;
}
/********************************************************************
 *
 *******************************************************************/
VOID HostMagInit(VOID)
{
    ObjCmdParserRegister(HostMagParser);
}
/********************************************************************
 * End of File
 *******************************************************************/




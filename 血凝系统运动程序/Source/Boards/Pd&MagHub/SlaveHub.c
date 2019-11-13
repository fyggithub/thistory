/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SlaveHub.c
 *******************************************************************/
#include "SlaveHub.h"
#include "BoardHost.h"
#include "PdState.h"
#include "MagState.h"
#include "Console.h"
#include "Message.h"
#include "Board.h"
#include "data.h"

#include <string.h>
#include "SlaveHub.h"
#include "Bits.h"
/********************************************************************
 *
 *******************************************************************/
STATIC UINT8 tag;
CommuObj pdSlaveObj;
CommuObj magSlaveObj;
/********************************************************************
 *
 *******************************************************************/
VOID ObjDataParserRegister (CommuObj* obj,DATAPARSER parser)
{
	if (parser){
        obj->dataParser = parser;
        DebugLog(tag, "Obj cmd Register Finished.");
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID BoardCommuInit(CommuObj* obj,UINT8 id)
{
    obj->uart = BoardGetSlaveUart(id);
	BufferCreate (&(obj->rxBuffer), obj->rxBufferArray, COMMU_RX_LEN);
	UartInit (obj->uart, &(obj->rxBuffer), obj->txArray, COMMU_RX_LEN);  
}

/********************************************************************
 *
 *******************************************************************/
BOOL ObjDataProcess (CommuObj* obj)
{
	UINT16 len = BufferLength (&(obj->rxBuffer));
	if (len >= 3) {
		BYTE header[3];
        UINT16 data[1];
        BufferPeek (&(obj->rxBuffer), (BYTE*)&header, len);
       // BufferPop(&(obj->rxBuffer),header,len);
        if(CheckData(header,data) == TRUE){
            BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)header;
            UINT8 sequence = ptrDataFrame->sequence;
            BufferPop(&(obj->rxBuffer),header,3);
            obj->dataParser(data,sequence);
            return TRUE;
        }
        else{
            BufferPop(&(obj->rxBuffer),header,1);
        }	
	}
    return FALSE;
}
/********************************************************************
 *
 *******************************************************************/ 
VOID SlaveDataProcess(VOID)
{
    ObjDataProcess(&pdSlaveObj);
    ObjDataProcess(&magSlaveObj);
}
/********************************************************************
 *
 *******************************************************************/ 
VOID SlaveSend(UINT8 id,BYTE* data, UINT16 len)
{
	if(id== PD_METHOD)
        UartSend (pdSlaveObj.uart, data, len);
    if(id == MAG_METHOD)
        UartSend (magSlaveObj.uart, data, len);
	DebugHex (tag, data, len);
}
/********************************************************************
 *
 *******************************************************************/ 
VOID SlaveInit(VOID)
{
    tag = DebugRegister ("obj");
    BoardCommuInit(&pdSlaveObj,PD_METHOD);
    ObjDataParserRegister (&pdSlaveObj,PdDataProcess);
    BoardCommuInit(&magSlaveObj,MAG_METHOD);
    ObjDataParserRegister (&magSlaveObj,MagDataProcess);
    DebugLog(tag, "Obj Init Finished.");
}
/********************************************************************
 * End of File
 *******************************************************************/




/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Host.c
 *******************************************************************/
#include "BoardHost.h"
#include "Bits.h"
STATIC UINT8 tag;
CommuObj hostObj;
/********************************************************************
 *
 *******************************************************************/
UINT8 ParityCheck(INT16 data)
{
    UINT8 parity = 0;
    for(UINT8 i=0; i<16; i++){
        parity ^= (data&0x1);
        data >>=1;
    }
    return (parity&0x1);
}
/********************************************************************
 *
 *******************************************************************/
VOID BoardCmdAck     (UINT8 sequnce,UINT16 data)
{
    BoardCommuFrame dataFrame;

    dataFrame.firstByteIdentify = FIRST_IDENTIFY;
    dataFrame.secondByteIdentify = SECOND_IDENTIFY;
    dataFrame.thirdByteIdentify = THIRD_IDENTIFY;
    dataFrame.check = ParityCheck(data);
    dataFrame.sequence = sequnce;
    dataFrame.hDataByte7_6 = HBYTE(data)>>6;
    dataFrame.hDataByte5_0 = HBYTE(data)&0x3F;
    dataFrame.lDataByte7   = LBYTE(data)>>7;
    dataFrame.lDataByte6_0 = LBYTE(data)&0x7F;
    
	UartSend (hostObj.uart, (BYTE*)&dataFrame, 3);
	DebugHex (tag, (BYTE*)&data, 2);
}

/********************************************************************
 *
 *******************************************************************/
VOID BoardCommuInit    (VOID)
{
	tag = DebugRegister ("obj");
    hostObj.uart = BoardGetHostUart();
	BufferCreate (&(hostObj.rxBuffer), hostObj.rxBufferArray, COMMU_RX_LEN);
	UartInit (hostObj.uart, &(hostObj.rxBuffer), hostObj.txArray, COMMU_RX_LEN);
    DebugLog(tag, "Obj Init Finished.");
}
/********************************************************************
 *
 *******************************************************************/
UINT16 GetSequence(BYTE* buffer)
{
    BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)buffer;
    UINT16 sequence = ptrDataFrame->sequence;
    return sequence;
}
/********************************************************************
 *
 *******************************************************************/
UINT16 GetData(BYTE* buffer)
{
    BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)buffer;
    INT16 data = (ptrDataFrame->hDataByte7_6<<14)
                +(ptrDataFrame->hDataByte5_0<<12)
                +(ptrDataFrame->lDataByte7<<7)+(ptrDataFrame->lDataByte6_0);
    return data;
}
/********************************************************************
 *
 *******************************************************************/
BOOL CheckData(BYTE* buffer,INT16* data)
{
    BOOL isValid = FALSE;
    BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)buffer;
    if((ptrDataFrame->firstByteIdentify == 0x1)
        && (ptrDataFrame->secondByteIdentify ==0x0)
        && (ptrDataFrame->thirdByteIdentify ==0x0)){
        INT16 d = GetData(buffer);
        *data = d;
        UINT8 parity = ParityCheck(d);
        if(parity == ptrDataFrame->check)
            isValid =TRUE;
    }
    return isValid;    
}
/********************************************************************
 *
 *******************************************************************/
VOID ObjCmdProcess (VOID)
{
	UINT16 len = BufferLength (&(hostObj.rxBuffer));
	if (len >= 1) {
		BYTE header;
		BufferPop (&(hostObj.rxBuffer), (BYTE*)&header, 1);
		hostObj.cmdParser(header);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID ObjDataProcess (INT16* data,UINT8* sequence)
{
	UINT16 len = BufferLength (&(hostObj.rxBuffer));
	if (len >= 3) {
		BYTE header[3];
        BufferPeek (&(hostObj.rxBuffer), (BYTE*)&header, 3);
        if(CheckData(header,data) == TRUE){
            BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)header;
            *sequence = ptrDataFrame->sequence;
            BufferPop(&(hostObj.rxBuffer),header,3);
        }
        else{
            BufferPop(&(hostObj.rxBuffer),header,1);
        }	
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID ObjCmdParserRegister (CMDPARSER parser)
{
	if (parser){
        hostObj.cmdParser = parser;
        DebugLog(tag, "Obj cmd Register Finished.");
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



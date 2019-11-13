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
    UINT16 data;
    BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)buffer;
    data = (ptrDataFrame->hDataByte7_6<<14)
                +(ptrDataFrame->hDataByte5_0<<8)
                +(ptrDataFrame->lDataByte7<<7)+(ptrDataFrame->lDataByte6_0);
    return data;
}
/********************************************************************
 *
 *******************************************************************/
BOOL CheckData(BYTE* buffer,UINT16* data)
{
    BOOL isValid = FALSE;
    INT16 d;
    UINT8 parity;
    BoardCommuFrame* ptrDataFrame = (BoardCommuFrame*)buffer;
    if((ptrDataFrame->firstByteIdentify == 0x1)
        && (ptrDataFrame->secondByteIdentify ==0x0)
        && (ptrDataFrame->thirdByteIdentify ==0x0)){
         d = GetData(buffer);
        *data = d;
        parity = ParityCheck(d);
        if(parity == ptrDataFrame->check)
            isValid =TRUE;
        if(isValid ==FALSE)
            isValid = isValid;
    }
    return isValid;    
}
/********************************************************************
 * END OF FILE
 *******************************************************************/



/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * MagState.c
 *******************************************************************/
#include "SlaveHub.h"
#include "GlobalVariableSet.h"
#include "Board.h"
#include "data.h"
#include "PdState.h"
#include "Uart.h"
#include "UartF103CB.h"
#include "led.h"


//UINT16 magData[1]    = {0};

//UINT16 magData[1];
/********************************************************************
 *
 *******************************************************************/
VOID MagCmdSend(VOID)
{
    BYTE cmd;
    if(GloVarHas(GLOBAL_MAG_DATA)){
        cmd = 'E';
        SlaveSend(MAG_METHOD,&cmd,1);
    }
}
/********************************************************************
 *
 *******************************************************************/
VOID MagDataProcess(UINT16* data, UINT8 sequence)
{   
    UINT16 magData = *data;
	if(GloVarHas(GLOBAL_MAG_DATA)){
        DataAdd(MAG_METHOD,(UINT8*)&magData);
    }
    return;
}
/********************************************************************
 * End of File
 *******************************************************************/





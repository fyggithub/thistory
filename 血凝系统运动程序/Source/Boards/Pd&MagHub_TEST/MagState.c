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
enum{
	PD_DEBUG_ON = 0,
	PD_DEBUG_OFF,
	PD_RESULT_DIV,
	PD_RESULT_ON,
	PD_RESULT_OFF,
};
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
    UINT16 magData;
	magData = *data;
	if(GloVarHas(GLOBAL_MAG_DATA)){
		DataAdd(MAG_METHOD,(UINT8*)&magData);
    }
	if(sequence == 0xF){
		switch(magData){
			case PD_DEBUG_ON:
				GloVarSet(GLOBAL_PD_DEBUG);
				break;
			case PD_RESULT_DIV:
				SetResult(0);
				break;			
			case PD_RESULT_ON:
				SetResult(1);
				break;			
			case PD_RESULT_OFF:
				SetResult(2);
				break;			
			case PD_DEBUG_OFF:
				GloVarClear(GLOBAL_PD_DEBUG);
				break;	
		}			
	}
    return;
}
/********************************************************************
 * End of File
 *******************************************************************/





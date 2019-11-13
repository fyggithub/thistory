/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * PdState.c
 *******************************************************************/
#include "SlaveHub.h"
#include "GlobalVariableSet.h"
#include "Board.h"
#include "data.h"
#include "PdState.h"
#include "Uart.h"
#include "UartF103CB.h"
#include "led.h"
#include "host.h"

UINT16 pdData[12]    = {0xFFFF};
UINT16 pdOnData[12]  = {0xFFFF};
UINT16 pdOffData[12] = {0xFFFF};
UINT16* showResult = NULL;
extern UINT8 pdStates;
UINT8 remember;

/********************************************************************
 *
 *******************************************************************/
VOID SetResult(UINT8 choose)
{
	switch(choose){
		case 0:
			showResult = pdData;
			break;
		case 1:
			showResult = pdOnData;
			break;
		case 2:
			showResult = pdOffData;
			break;		
		default:
			break;
	}
}
/********************************************************************
 *
 *******************************************************************/
UINT8 PdCmdSend(UINT32 state)
{
    BYTE  cmd;
    UINT8 time = 0;
    switch(state){
        case PD_LED_ON:
            if(GloVarHas(GLOBAL_LED_ON)){
                cmd = 'A';
                SlaveSend(PD_METHOD,&cmd,1);
				TimerTest();
            }
            time = 20;
            break;
        case PD_LED_ON_READ:
            if(GloVarHas(GLOBAL_ON_DATA | GLOBAL_DIV_DATA)){
                cmd = 'D';
                SlaveSend(PD_METHOD,&cmd,1);
            }
            time = 5;
            break;
        case PD_LED_OFF:
            if(GloVarHas(GLOBAL_LED_ON)){
                cmd = 'B';           
                SlaveSend(PD_METHOD,&cmd,1);
				TimerTest();
            }
            time = 35;
            break;
        case PD_LED_OFF_READ:
            if(GloVarHas(GLOBAL_OFF_DATA | GLOBAL_DIV_DATA)){
                cmd = 'D';
                SlaveSend(PD_METHOD,&cmd,1);               
            }
            time = 40;
            break;
        default:
            time = 10;
            break;
    }
    return time;
}
/********************************************************************
 *
 *******************************************************************/
VOID PdCmdCharSend(BYTE cmd)
{
    SlaveSend(PD_METHOD,&cmd,1);
}
/********************************************************************
 *
 *******************************************************************/
VOID PdDataProcess(UINT16* data, UINT8 sequence)
{   
    CHAR debugData[12][7];		
	BYTE enter = 0x0A;
	if(sequence>11)
        return;  
    if((pdStates == PD_LED_ON_READ)|(pdStates ==PD_LED_OFF))
        pdOnData[sequence] = *data;
    else if(pdStates == PD_LED_OFF_READ)
        pdOffData[sequence] = *data;
    if(sequence != 11)
        return;
   
    if(GloVarHas(GLOBAL_OFF_DATA)){
        DataAdd(PD_METHOD,(UINT8*)&pdOffData);
    }
    else if(GloVarHas(GLOBAL_ON_DATA)){
        DataAdd(PD_METHOD,(UINT8*)&pdOnData);
    }
    else if(GloVarHas(GLOBAL_DIV_DATA)){
        if(pdStates == PD_LED_OFF_READ){
            for(UINT8 i=0; i<12; i++){
                pdData[i] = pdOnData[i]- pdOffData[i];
				if( pdOnData[i]>pdOffData[i])
					pdData[i] = pdOnData[i]- pdOffData[i];
				else
					pdData[i] = 0;
				sprintf(debugData[i],"%5u" ,showResult[i]);
				debugData[i][5]=' ';
            }
			
			if(GloVarHas(GLOBAL_PD_DEBUG)){				
				for(UINT8 i=0; i<12; i++){
					SlaveSend(MAG_METHOD,(UINT8*)debugData[i],6);
				}
				SlaveSend(MAG_METHOD,&enter,1);
			}
			if(GloVarHas(GLOBAL_PD_STORE)){
				DataAdd(PD_METHOD,(UINT8*)&pdData);
			}
		} 
    }
    return;
}
/********************************************************************
 *
 *******************************************************************/
VOID PdDataStop(VOID)
{

}
/********************************************************************
 * End of File
 *******************************************************************/





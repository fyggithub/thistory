/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Hub.c
 *******************************************************************/
#include "Hub.h"
#include "Board.h"
#include "Console.h"
#include "Host.h"
#include "SlaveHub.h"
#include "HostHub.h"
#include "data.h"
#include "string.h"
#include "PdTimer.h"
#include "MagTimer.h"
#include "GlobalVariableSet.h"
#include "PdState.h"
#include "MagState.h"
/********************************************************************
 *
 *******************************************************************/
#define DATA_LENGTH 50
#define HOST_DATA_LENGTH 10
volatile UINT8 pdStates = PD_LED_ON;
      
UINT8  data[DATA_LENGTH];

 /********************************************************************
 *
 *******************************************************************/
STATIC VOID HubMessageHandler(Message* msg)
{
    UINT8  hostData[HOST_DATA_LENGTH];
    UINT32 x = 0;
    UINT16 y = 0;
    UINT8  z = 0;
    UINT8 *w = NULL;
	switch(msg->id){
        case MSG_HOST_GETINFO:
            DebugLog(tag, "GetInfo()");
            break;
        case MSG_HOST_SETINFO:
            DebugLog(tag, "SetInfo()");
            break;
        case MSG_HOST_LED_ON:
            GloVarSet(GLOBAL_LED_ON);
            pdStates = PD_LED_ON;
            z = PdCmdSend(PD_LED_ON);
            PdTimerStart(z);
            break;
        case MSG_HOST_LED_OFF:
            GloVarClear(GLOBAL_LED_ON);
            PdCmdCharSend('B');				
			Message msg1;
			msg1.id = MSG_PD_DEBUG_ON;
			MessagePost(&msg1);
            break;
        case MSG_HOST_PD_DETECT_ON:
            if(msg->p1>0) DataReset(PD_METHOD);
            x = 0x02 << msg->p2;//related to GLOBAL_VAR
            GloVarClear(GLOBAL_ON_DATA|GLOBAL_OFF_DATA|GLOBAL_DIV_DATA);  
            GloVarSet(x);  
			GloVarSet(GLOBAL_PD_STORE);
            pdStates = PD_LED_ON;
            z = PdCmdSend(PD_LED_ON);
            PdTimerStart(z);
            
            x = DataFirstTime(PD_METHOD);
            hostData[0] = 0;
            w = &hostData[1];
            ARRAYSET(w,x);
            HostResponse(hostData,4); 						
            break;		
        case MSG_HOST_PD_DETECT_OFF:
            GloVarClear(GLOBAL_ON_DATA|GLOBAL_OFF_DATA|GLOBAL_DIV_DATA);   
            x = DataLastTime(PD_METHOD);
            hostData[0] = 0;
            w = &hostData[1];
            ARRAYSET(w,x);
            HostResponse(hostData,4);
            break;		
        case MSG_HOST_PD_DATA:
            x  = (UINT32)msg->p1;//start time
            y  = (UINT16)msg->p2;//data length
            y  = DataPeek(PD_METHOD,data,x,y);//response data number
            HostResponse(data,y);	
            break;		
        case MSG_HOST_PD_TIME:
            hostData[0] = 0;
        
            w = &hostData[1];
			x = DataLastTime(PD_METHOD);//current time index
            ARRAYSET(w,x);
            
            hostData[4] = PdTimerAskTime(pdStates);//current time
            HostResponse(hostData,5);
            break;		
        case MSG_PD_TIMER: 
            pdStates++;
            if(pdStates == PD_MAX_STATE){
                pdStates = PD_LED_ON;    
            } 
            z = PdCmdSend(pdStates);
            PdTimerStart(z);
            break;		
        case MSG_HOST_MAG_START:
            GloVarSet(GLOBAL_MAG_DATA);
            MagTimerStart();
            break;
        case MSG_HOST_MAG_STOP:
            GloVarClear(GLOBAL_MAG_DATA);
            break;
        case MSG_HOST_MAG_READ:
            x = (UINT32)msg->p1;
            y = (UINT16)msg->p2;
            y = DataPeek(MAG_METHOD,data,x,y);
            HostResponse(data,y);	
            break;
        case MSG_MAG_TIMER:
            MagCmdSend();
            MagTimerStart();
            break;
		case MSG_PD_DEBUG_ON:
			GloVarClear(GLOBAL_ON_DATA|GLOBAL_OFF_DATA|GLOBAL_DIV_DATA|GLOBAL_PD_STORE);
			GloVarSet(GLOBAL_LED_ON);
			GloVarSet(GLOBAL_DIV_DATA);
			GloVarSet(GLOBAL_PD_DEBUG);  				
			pdStates = PD_LED_ON;
			z = PdCmdSend(PD_LED_ON);
			PdTimerStart(z);
			break;
		case MSG_PD_DEBUG_OFF:
			GloVarClear(GLOBAL_PD_DEBUG);  
			break;
        default:
            break;
	}
}
VOID HubInit(VOID)
{ 
    DataInit();
	SetResult(0);
	MessageRegisterHandler (HubMessageHandler);
	Message msg;
	msg.id = MSG_PD_DEBUG_ON;
	MessagePost(&msg);
}
/********************************************************************
 * End of File
 *******************************************************************/




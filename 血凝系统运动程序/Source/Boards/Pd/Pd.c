/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Pd.c
 *******************************************************************/
#include "Pd.h"
#include "Board.h"
#include "Console.h"
#include "TimerPd.h"
#include "host.h"
#include "led.h"
#include "AD7689.h"
#include "BoardHost.h"

/********************************************************************
 *
 *******************************************************************/
#define PD_IDENTIFY			(0xE1C3)
#define PD_ADDR             (0x0F)
#define PD_TYPE_ACK			(0x11)
#define PD_TYPE_RESULT      (0x12)
#define PD_TX_LEN           64

/********************************************************************
 *
 *******************************************************************/
 #define COMMU_RX_LEN		(64)
#define COMMU_TX_LEN		(64)
/********************************************************************
 *
 *******************************************************************/


STATIC UINT8 tag=0;
UINT16 x = 0;
/********************************************************************
 *
 *******************************************************************/
STATIC VOID PdMessageHandler(Message* msg)
{
    UINT16 ch[12] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,
                     0xFFFF,0xFFFF,0xFFFF,0xFFFF,   
                     0xFFFF,0xFFFF,0xFFFF,0xFFFF};
    UINT16 test[12]={0x1234,0x1234,0x1234,0x1234,
                     0x1234,0x1234,0x1234,0x1234,
                     0x1234,0x1234,0x1234,0x1234,
                     };
    switch(msg->id){
		case MSG_HOST_GETINFO:
			DebugLog(tag, "GetInfo()");
			break;
		case MSG_HOST_SETINFO:
			DebugLog(tag, "SetInfo()");
			break;
		case MSG_HOST_LED_ON:
            LedSwitch(LEDON);
            BoardCmdAck(ACK_SUCCESS,0x1111);
			break;
		case MSG_HOST_LED_OFF:
			
			LedSwitch(LEDOFF);
            BoardCmdAck(ACK_SUCCESS,0x2222);
			break;
		case MSG_HOST_PD_DETECT:
			LEDtest(0);
            AD7689Get(ch);

            //if(x >= 100) return;
            for(UINT8 i=0; i<12; i++){
                BoardCmdAck(i,ch[i]);
                //BoardCmdAck(i,x);
            }
			LEDtest(1);
			break;	
		default:
			break;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID PDInit(VOID)
{
	tag = DebugRegister ("Pd");
	MessageRegisterHandler (PdMessageHandler);
	DebugLog(tag, "Pd App Init.");
    BoardCommuInit();
}

/********************************************************************
 * End of File
 *******************************************************************/




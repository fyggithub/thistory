/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Mag.c
 *******************************************************************/
#include "Mag.h"
#include "Console.h"
#include "Timer.h"
#include "AD7928.h"
#include "BoardHost.h"
#include "HostMag.h"
#include "stdio.h"
/********************************************************************
 *
 *******************************************************************/

#define TIMER_PERIOD_MS		(80) /// 1000/80/4 = 3.125Hz

enum {PHASE1, PHASE2, PHASE3, PHASE4};

typedef struct _MagApp
{
	Gpio*  led;
	Gpio*  en1;
	Gpio*  en2;
	Gpio*  en3;
	Gpio*  en4;
	UINT32 phase;
	BOOL   ADisOpen;
}MagApp;

//STATIC UINT8 tag;
STATIC MagApp magApp;

/********************************************************************
 *
 *******************************************************************/

STATIC VOID MagDirect(INT32 dir)
{
	if(dir == 1){
		GpioSet(magApp.en1, 1);
		GpioSet(magApp.en2, 0);
		GpioSet(magApp.en3, 1);
		GpioSet(magApp.en4, 0);
	}
	else if(dir == -1){
		GpioSet(magApp.en1, 0);
		GpioSet(magApp.en2, 1);
		GpioSet(magApp.en3, 0);
		GpioSet(magApp.en4, 1);
	}
	else{ // dir == 0
		GpioSet(magApp.en1, 0);
		GpioSet(magApp.en2, 0);
		GpioSet(magApp.en3, 0);
		GpioSet(magApp.en4, 0);
	}
}

/********************************************************************
 *
 *******************************************************************/

STATIC VOID MessageOnTimer(VOID)
{
	switch(magApp.phase){
	case PHASE1:
		MagDirect (1);
		GpioSet(magApp.led, 0); // ON
		break;
	case PHASE2:
		MagDirect (0);
		GpioSet(magApp.led, 0); // ON
		break;
	case PHASE3:
		MagDirect (-1);
		GpioSet(magApp.led, 1); // OFF
		break;
	case PHASE4:
		MagDirect (0);
		GpioSet(magApp.led, 1); // OFF
		break;
	}

	magApp.phase++;
	if(magApp.phase > PHASE4)
		magApp.phase = PHASE1;
}

/********************************************************************
 *使用if(magApp.ADisOpen == FALSE)来控制磁珠板的PWM波是否开启
*前提条件：开启磁珠法后，每次PWM电平翻转前，必定发生AD采集。AD采集的频率是10ms,
远远小于PWM的周期
 *******************************************************************/
STATIC VOID MagMessageHandler(Message* msg)
{
    switch(msg->id){
	case MSG_TIMER:
		if(magApp.ADisOpen == FALSE){
			MagDirect(0);
			TimerStart(TIMER_PERIOD_MS, 1);	
			return;
		}
		MessageOnTimer();
		TimerStart(TIMER_PERIOD_MS, 1);	
		magApp.ADisOpen = FALSE;
		break;
    case MSG_HOST_MAG_DATA:
        {
			WORD ad[4];
			AD7928Update ();
			ad[0] = AD7928Read (0);
			ad[1] = AD7928Read (1);
			ad[2] = AD7928Read (2);
			ad[3] = AD7928Read (3);
            BoardCmdAck(0,ad[0]);
            //sprintf(s,"%d",ad[0]);
			magApp.ADisOpen = TRUE;		
			//DebugLog(tag, "%d, %d, %d, %d", ad[0], ad[1], ad[2], ad[3]);
        }
	default:
		break;
	}
}

/********************************************************************
 *
 *******************************************************************/

VOID MagInit(VOID)
{
	//tag = DebugRegister("mag");
    HostMagInit();
	MessageRegisterHandler (MagMessageHandler);

	magApp.led = BoardGetGpio (IO_LED1);
	magApp.en1 = BoardGetGpio (IO_M1ENA);
	magApp.en2 = BoardGetGpio (IO_M2ENA);
	magApp.en3 = BoardGetGpio (IO_M3ENA);
	magApp.en4 = BoardGetGpio (IO_M4ENA);

	magApp.phase = PHASE1;
	MagDirect(0);
	TimerInit();
	TimerStart(TIMER_PERIOD_MS, 1);	
	AD7928Init ();
	magApp.ADisOpen = FALSE;
	//DebugLog(tag, "Mag App Init Finished.");
}



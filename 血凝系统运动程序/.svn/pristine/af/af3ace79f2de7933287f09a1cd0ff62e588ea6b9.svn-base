/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Main.c
 *******************************************************************/
#include "Datatypes.h"
#include "Board.h"
#include "Console.h"
#include "Message.h"

#include "HostPd.h"
#include "Gpio.h"
#include "BoardHost.h"
#include "led.h"
#include "Pd.h"
#include "AD7689.h"

INT32 main()
{
	// Init Hardware
	BoardInit();
	ConsoleInit();
	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
    
	//==================== Init System
	MessageQueueInit ();
	//TimerInit ();


	LedInit();
    HostPdInit();
	PDInit();      
	
	GpioSet(BoardGetGpio(IO_TESTLED),0);  
	//==================== Init System
	// Do Loop
	
	while(TRUE){
		MessageQueueProcess();
        ObjCmdProcess();
		ConsoleProcess();
	}
}
/********************************************************************
 * End of File
 *******************************************************************/


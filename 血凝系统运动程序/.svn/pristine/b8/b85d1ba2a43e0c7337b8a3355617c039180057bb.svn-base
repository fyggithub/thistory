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
#include "Host.h"
#include "Gpio.h"
#include "PdTimer.h"
#include "MagTimer.h"
#include "HostHub.h"
#include "SlaveHub.h"
#include "Config.h"
#include "Hub.h"
#include "PdState.h"

INT32 main()
{
	BoardInit();
	ConsoleInit();
	//==================== Init System
	//EventManagerInit ();
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
    MessageQueueInit ();
    //TimerInit ();

    HostInit ();
    HostPdMainInit ();
    SlaveInit();

    MagTimerInit();
    HubInit();
		
	//==================== Init System
	while(TRUE){		
        SlaveDataProcess();
        MessageQueueProcess();
        HostProcess();
		ConsoleProcess();
	}
}
/********************************************************************
 * End of File
 *******************************************************************/



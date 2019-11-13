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
#include "Timer.h"
#include "Config.h"
#include "StateMonitor.h"
#include "HostStateMonitor.h"

INT32 main()
{
	// Init Hardware
	BoardInit();
	ConsoleInit();
	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
	TimerInit();
	//==================== Init System
	MessageQueueInit ();
	        HostInit ();
	HostStateMonitorInit();
	StateMonitorInit();

	//==================== Init System
	// Do Loop
	while(TRUE){
		MessageQueueProcess();
		HostProcess();
		ConsoleProcess();
	}
}


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
#include "StepMotor.h"
#include "AccelerateTable.h"
#include "HostSample.h"
#include "Sample.h"

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
	       TimerInit ();
		   TimerInit2();
		  ConfigInit ();
	        HostInit ();
	   HostSampleInit();
		   SampleInit();
	//==================== Init System
	// Do Loop

	while(TRUE){
		MessageQueueProcess();
		HostProcess();
		ConsoleProcess();
		// Maybe change to low power mode
		// PowerSaveMode();
	}
}


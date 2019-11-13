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
#include "Cool.h"
#include "Pwm.h"
#include "HostCool.h"

INT32 main()
{
	// Init Hardware
	BoardInit();
//        PwmInit();
//   PwmSetDutyRatio(500);
	ConsoleInit();
//	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);

//	//==================== Init System
	MessageQueueInit ();
	        HostInit ();
			CoolInit ();
    HostCoolInit();
//	//==================== Init System
//	// Do Loop
	while(TRUE){
		MessageQueueProcess();
		HostProcess();
		ConsoleProcess();
		// Maybe change to low power mode
		// PowerSaveMode();
	}
}


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
#include "HostLoad.h"
#include "Config.h"
#include "Load.h"

#include "boot_addr_F103ZE.h"



static const IAPHeader IapHeader IAP_HEADER_FLASH_ADDR_ATTRIB = 
{
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	VERSION,
};

INT32 main()
{
	// Init Hardware
	BoardInit();
	ConsoleInit();
	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
	DebugLog    (0, "%x",VERSION);
	//==================== Init System
	MessageQueueInit ();
	       TimerInit ();
	      ConfigInit ();
	        HostInit ();
	    HostLoadInit ();
		    LoadInit ();
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


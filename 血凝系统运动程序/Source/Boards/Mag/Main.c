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
#include "Mag.h"
#include "AD7928.h"
#include "BoardHost.h"

INT32 main()
{
	// Init Hardware
	BoardInit();
	AD7928Init();
	ConsoleInit();
	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
    BoardCommuInit();
	//==================== Init System
	MessageQueueInit ();
	MagInit();
	//==================== Init System
	// Do Loop
	while(TRUE){
		MessageQueueProcess();
		ConsoleProcess();
        ObjCmdProcess();
	}
}
/********************************************************************
 * End of File
 *******************************************************************/


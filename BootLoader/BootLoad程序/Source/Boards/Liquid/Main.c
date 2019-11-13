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
#include "Liquid.h"
#include "Adc.h"
#include "HostLiquid.h"
#include "Tim4.h"
#include "LiquidPwm.h"
INT32 main()
{
	// Init Hardware
	BoardInit();
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	ConsoleInit();
    HostInit();
	// Write Message to Console
	DebugWarning(0, "***********************");
	DebugLog    (0, "Sys clock = %d", SystemCoreClock);
    
	//==================== Init System
    MessageQueueInit ();

    SpiInit(BoardGetSpiExt());
    AdcStart();
    
    HostLiquidInit();
    Timer4Start();
    LiquidInit();
    //UINT8   data[2];
	//==================== Init System
	// Do Loop
	while(TRUE){
        //HostResponse(data,2);
		MessageQueueProcess();
		HostProcess();
		ConsoleProcess();
        LiquidProcess();
		// Maybe change to low power mode
		// PowerSaveMode();
	}
}


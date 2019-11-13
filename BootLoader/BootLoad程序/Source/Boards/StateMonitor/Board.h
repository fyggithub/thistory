/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Board.h
 *******************************************************************/
#ifndef _BOARD_H_
#define _BOARD_H_
/********************************************************************
 * Macro defined
 *******************************************************************/
#include "Datatypes.h"
#include "Uart.h"
#include "Uart4Axis.h"
#include "Gpio.h"
#include "Spi.h"

/********************************************************************
 *
 *******************************************************************/

#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/

/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
	MSG_INIT,
	MSG_HOST_VERSION = 20,
	MSG_HOST_GETINFO,
	MSG_HOST_SETINFO,
	MSG_HOST_PARINFO,
	MSG_HOST_STATE_ASK,
	MSG_HOST_STATE_LED_CONTROL,
	MSG_HOST_EMERGENCY_STOP,
	
	MSG_HOST_UPDATEASK,
	MSG_HOST_UPTRAN,
	
	MSG_READSAM1,
	MSG_READSAM2,
	MSG_READDOOR,
	MSG_READCLEAN,
	MSG_READWASTE,
	MSG_OPEN_LEDON,
	MSG_OPEN_LEDOFF,
	MSG_TEST,
	
	MSG_TIMER,
	MSG_TIMER2,
	
	MSG_CONT1,
	MSG_CONT2,
	MSG_CONT3,

	MSG_PSD,
	MSG_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define EN_CONSOLE

#define EN_UART01
// #define EN_UART02
#define EN_UART03
// #define EN_UART04
// #define EN_UART05

#define EN_PULSE01_IRQ
#define EN_PULSE02_IRQ
// #define EN_PULSE03_IRQ
// #define EN_PULSE04_IRQ

/********************************************************************
 *
 *******************************************************************/

enum{
	IO_SAM_OPTO1 = 0, IO_SAM_OPTO2, IO_SAM_OPTO3, IO_SAM_OPTO4,
	IO_SAM_OPTO5, IO_SAM_OPTO6, IO_SAM_OPTO7, IO_SAM_OPTO8, 
	
	IO_SAM_LED1 = 8, IO_SAM_LED2, IO_SAM_LED3, IO_SAM_LED4,
	IO_SAM_LED5, IO_SAM_LED6, IO_SAM_LED7, IO_SAM_LED8, 	

	IO_DOOR_OPTO,IO_ClEAN_FLOAT,IO_WASTE_FLOAT,

	IO_EMERGENCY_KEY,
	
	IO_LED1,IO_LED2,
	IO_MAX,
};


/********************************************************************
 *
 *******************************************************************/
Uart*   BoardGetConsoleUart(VOID);
Uart*   BoardGetHostUart(VOID);
Gpio*   BoardGetGpio(UINT16 id);
Spi*    BoardGetSpiFRam(VOID);
Spi*    BoardGetSpiExt(VOID);
UINT16 MonitorGpio(UINT8 i);
VOID    BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

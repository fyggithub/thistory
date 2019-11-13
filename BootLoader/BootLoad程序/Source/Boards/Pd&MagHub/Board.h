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
#include "UartF103CB.h"
#include "Gpio.h"
#include "Spi.h"
#include "Pulse.h"
/********************************************************************
 *
 *******************************************************************/
//#define _CLOCK36M
#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/

enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
    MSG_TIMER2,
    MSG_TEST,
    MSG_HOST_VERSION = 20,
    MSG_HOST_GETINFO,
    MSG_HOST_SETINFO,
    MSG_HOST_PARINFO,
    
    MSG_HOST_LED_ON,
    MSG_HOST_LED_OFF,
    
    MSG_HOST_PD_DETECT_ON,
    MSG_HOST_PD_DETECT_OFF,
    MSG_HOST_PD_DATA,
    MSG_HOST_PD_TIME,
   
    MSG_HOST_MAG_START,
    MSG_HOST_MAG_STOP,
    MSG_HOST_MAG_READ,

    
    MSG_PD_TIMER,
    MSG_MAG_TIMER,
    
    MSG_HOST_FAULT,
    MSG_MAX,
};
/********************************************************************
 *
 *******************************************************************/
enum{
	IO_LEDTEST= 0,IO_MAX,
};
/********************************************************************
 *
 *******************************************************************/
//#define EN_CONSOLE
#define EN_UART01
#define EN_UART02
#define EN_UART03
#define EN_UART01_IRQ
#define EN_UART02_IRQ
#define EN_UART03_IRQ
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetConsoleUart(VOID);
Uart* BoardGetHostUart(VOID);
Uart* BoardGetSlaveUart(UINT8 id);
Gpio* BoardGetGpio(UINT16 id);
VOID  BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

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
#include "UartF051X8.h"
#include "Spi.h"
/********************************************************************
 *
 *******************************************************************/
#define BOARD_ADDR 0x0F
/********************************************************************
 *
*******************************************************************/
#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/
#define EV_MESSAGE			BIT(0)
#define EV_TIMER			BIT(1)
#define EV_PD_TIME			BIT(2)
#define EV_PD_DATA			BIT(3)
#define EV_PD_80MS          BIT(4)
#define EV_PD_0MS           BIT(5)


/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
	MSG_TIMER,
	MSG_TEST,
	MSG_HOST_VERSION = 20,
	MSG_HOST_GETINFO,
	MSG_HOST_SETINFO,
	MSG_HOST_PARINFO,
	MSG_HOST_LED_ON,
	MSG_HOST_LED_OFF,
	MSG_HOST_PD_DETECT,
	MSG_HOST_FAULT,
	MSG_MAX,
};
/********************************************************************
 *
 *******************************************************************/
#define EN_CONSOLE
#define EN_UART01
#define EN_UART02
#define EN_UART01_IRQ
#define EN_UART02_IRQ



enum{
	IO_LEDCON1 = 0,
	IO_LEDCON2,IO_LEDCON3,IO_LEDCON4,IO_LEDCON5,IO_LEDCON6, 
	IO_CNV_ADC1,IO_CNV_ADC2,IO_TESTLED,IO_SPI_MOSI,CSADC1,CSADC2,
	IO_MAX,
};
/********************************************************************
 *
 *******************************************************************/

Uart* BoardGetConsoleUart(VOID);
Uart* BoardGetHostUart(VOID);
Gpio* BoardGetGpio(UINT16 id);
Spi*  BoardGetSpiExt(VOID);
VOID TimerTest(VOID);
VOID LEDtest(UINT8 s);
VOID  BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

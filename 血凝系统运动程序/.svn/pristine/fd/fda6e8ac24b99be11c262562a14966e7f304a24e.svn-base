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
#include "Pulse.h"
#include "ConfigLoad.h"
/********************************************************************
 *
 *******************************************************************/
#define PULSE_NVIC_PRIOR (1)
#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/
#define EV_MESSAGE			BIT(0)
#define EV_TIMER			BIT(1)
#define EV_TIMER2			BIT(2)
#define EV_MOTOR_DIS_DONE	BIT(3)
#define EV_MOTOR_LOD_DONE	BIT(4)

/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
	MSG_TIMER,
	MSG_TIMER2,
	MSG_CFG_RESET,
	MSG_INIT,
	MSG_DISP_DONE,
	MSG_LOAD_DONE,

	MSG_HOST_VERSION = 20,
	MSG_HOST_GETINFO,
	MSG_HOST_SETINFO,
	MSG_HOST_PARINFO,
	MSG_HOST_PARSET,
	MSG_HOST_WRKSAVE,
	MSG_HOST_MOTOR_ENABLE,
	MSG_HOST_MOTOR_RESET,
	MSG_HOST_MOTOR_TOZERO,
	MSG_HOST_MOTOR_MOVREL,
	MSG_HOST_MOTOR_MOVABS,
	MSG_HOST_MOTOR_MOVWRK,
	MSG_HOST_MOTOR_GETPOS,
	MSG_HOST_LOADCUP,
	MSG_HOST_DISPATCH,
	MSG_HOST_IOSET,
	MSG_HOST_IOGET,
	MSG_HOST_OPTOGET,
	MSG_HOST_FAULT,
	MSG_HOST_STOP,
	MSG_HOST_DELAY,

	MSG_CMD_MOVREL = 50,
	MSG_CMD_MOVABS,
	MSG_CMD_RESET,
	MSG_CMD_DISP,
	MSG_CMD_LOAD,
	MSG_CMD_POS,
	MSG_CMD_OPTO,
	MSG_CMD_CONFIG,
	MSG_CMD_CFGSET,

	MSG_TEST,

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
	IO_LPD1 = 0,
	IO_LPD2,  //IO_LPD3,  IO_LPD4,
	IO_ORI1,  IO_ORI2,  //IO_ORI3,  IO_ORI4,
	IO_OPTO1, IO_OPTO2, IO_OPTO3, IO_OPTO4, IO_OPTO5, IO_OPTO6,
	IO_MAX,
};

// 滑道检测使用TIM4，和PULSE04 冲突，不可同时用
#define SLIDE_DETECT_IRQ_TIM		TIM4
#define SLIDE_DETECT_IRQN			TIM4_IRQn
#define SLIDE_DETECT_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define SLIDE_DETECT_IRQ_HANDLE		TIM4_IRQHandler

#define WORK2_OPTO	IO_OPTO3
#define WORK4_OPTO	IO_OPTO4
#define SLIDE_DETECT_UPPERIO	IO_OPTO5
#define SLIDE_DETECT_LOWERIO	IO_OPTO6

/********************************************************************
 *
 *******************************************************************/
Uart*   BoardGetConsoleUart(VOID);
Uart*   BoardGetHostUart(VOID);
Gpio*   BoardGetGpio(UINT16 id);
Spi*    BoardGetSpiFRam(VOID);
Spi*    BoardGetSpiExt(VOID);
Pulse*  BoardGetPulseDispatch(VOID);
Pulse*  BoardGetPulseLoad(VOID);

VOID    BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_

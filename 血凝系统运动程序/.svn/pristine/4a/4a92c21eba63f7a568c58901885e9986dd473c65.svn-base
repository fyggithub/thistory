/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Board.c
 *******************************************************************/

#include "Board.h"


/********************************************************************
 *
 *******************************************************************/
typedef struct __Board{
    Uart* uartConsole;
    Uart* uartHost;
    Uart* uartSlavePd;
    Uart* uartSlaveMag;
    Gpio  gpios[IO_MAX];
}Board;

STATIC Board board;
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetConsoleUart(VOID)
{
	return board.uartConsole;
}
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetHostUart(VOID)
{
	return board.uartHost;
}
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetSlaveUart(UINT8 id)
{
    if(id==0)
        return board.uartSlavePd;
    else
		return board.uartSlaveMag;
}
/********************************************************************
 *
 *******************************************************************/
Gpio* BoardGetGpio(UINT16 id)
{
	if (id < IO_MAX)
		return &(board.gpios[id]);
	return NULL;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID BoardGpioInit(VOID)
{

    board.gpios[IO_LEDTEST].io  = GPIOA;
	board.gpios[IO_LEDTEST].pin = GPIO_PIN_15;
    
	GpioInit(&(board.gpios[IO_LEDTEST]),
    GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    __HAL_RCC_AFIO_CLK_ENABLE();
     __HAL_AFIO_REMAP_SWJ_NOJTAG();
    HAL_GPIO_WritePin(board.gpios[IO_LEDTEST].io,board.gpios[IO_LEDTEST].pin, GPIO_PIN_RESET);   
}
/********************************************************************
 * Rcc Init
 *******************************************************************/
STATIC VOID RccInit(VOID)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	BOOL a = 0;
#ifdef USE_8M
	a = 1;
#endif
#if a
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 			= RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue 	= RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL 		= RCC_PLL_MUL4;
#else
	RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL9;
    //
#endif
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             	 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/********************************************************************
 *
 *******************************************************************/
VOID  BoardInit(VOID)
{
	HAL_Init();
	RccInit();
    BoardGpioInit();
	board.uartHost    = UartGetObject (UART01);
	board.uartConsole = NULL;
    board.uartSlaveMag= UartGetObject (UART02);
	board.uartSlavePd	  = UartGetObject (UART03);
}

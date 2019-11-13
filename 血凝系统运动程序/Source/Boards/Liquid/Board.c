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
	Gpio  gpios[IO_MAX];
    Spi*  spiExt;
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
Spi*  BoardGetSpiExt(VOID)
{
	return board.spiExt;
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

Gpio* BoardGetGpio(UINT16 id)
{
	if (id < IO_MAX)
		return &(board.gpios[id]);
	return NULL;
}
/********************************************************************
 * Rcc Init
 *******************************************************************/
STATIC VOID RccInit(VOID)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
#if 1
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 			= RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue 	= RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL 		= RCC_PLL_MUL6;
#else
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#endif
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             	 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;
    
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID BoardGpioInit(VOID)
{
	board.gpios[IO_LED1].io  = GPIOB;
	board.gpios[IO_LED1].pin = GPIO_PIN_8;
	GpioInit(&(board.gpios[IO_LED1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    
	board.gpios[PWM_GPIO].io  = GPIOB;
	board.gpios[PWM_GPIO].pin = GPIO_PIN_10;
	GpioInit(&(board.gpios[PWM_GPIO]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    
	board.gpios[IO_LED2].io  = GPIOB;
	board.gpios[IO_LED2].pin = GPIO_PIN_11;
	GpioInit(&(board.gpios[IO_LED2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    
    board.gpios[F2LIQUID].io  = GPIOB;
	board.gpios[F2LIQUID].pin = GPIO_PIN_6;
	GpioInit(&(board.gpios[F2LIQUID]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
//    GpioExitInit(&(board.gpios[F2LIQUID]), 
//        GPIO_MODE_IT_FALLING, GPIO_PULLUP,EXTI9_5_IRQn);
    
    board.gpios[L2FOURAXIS].io  = GPIOB;
	board.gpios[L2FOURAXIS].pin = GPIO_PIN_7;
	GpioInit(&(board.gpios[L2FOURAXIS]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);     
}
/********************************************************************
 *
 *******************************************************************/

VOID  BoardInit(VOID)
{
	HAL_Init();
	RccInit();
	BoardGpioInit ();
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	board.uartHost    = UartGetObject (UART01);
	board.uartConsole = UartGetObject (UART02);
    board.spiExt      = SpiGetObject  (SPI02);
    SpiInit(board.spiExt);
}

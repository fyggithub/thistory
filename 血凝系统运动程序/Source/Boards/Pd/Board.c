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
Uart* BoardGetHostUart(VOID)
{
	return board.uartHost;
}
/********************************************************************
 *
 *******************************************************************/


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
Spi*  BoardGetSpiExt(VOID)
{
	return board.spiExt;
}

/********************************************************************
 * Rcc Init
 *******************************************************************/
STATIC VOID RccInit(VOID)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
#if 0
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
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#endif
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             	 |RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID BoardGpioInit(VOID)
{

    board.gpios[IO_LEDCON1].io  = GPIOA;
	board.gpios[IO_LEDCON1].pin = GPIO_PIN_8;
	GpioInit(&(board.gpios[IO_LEDCON1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON1].io,board.gpios[IO_LEDCON1].pin, GPIO_PIN_SET);

	board.gpios[IO_LEDCON2].io  = GPIOA;
	board.gpios[IO_LEDCON2].pin = GPIO_PIN_9;
	GpioInit(&(board.gpios[IO_LEDCON2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON2].io,board.gpios[IO_LEDCON2].pin, GPIO_PIN_SET);

    board.gpios[IO_LEDCON3].io  = GPIOA;
	board.gpios[IO_LEDCON3].pin = GPIO_PIN_10;
	GpioInit(&(board.gpios[IO_LEDCON3]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON3].io,board.gpios[IO_LEDCON3].pin, GPIO_PIN_SET);

    board.gpios[IO_LEDCON4].io  = GPIOA;
	board.gpios[IO_LEDCON4].pin = GPIO_PIN_11;
	GpioInit(&(board.gpios[IO_LEDCON4]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON4].io,board.gpios[IO_LEDCON4].pin, GPIO_PIN_SET);

    board.gpios[IO_LEDCON5].io  = GPIOA;
	board.gpios[IO_LEDCON5].pin = GPIO_PIN_12;
	GpioInit(&(board.gpios[IO_LEDCON5]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON5].io,board.gpios[IO_LEDCON5].pin, GPIO_PIN_SET);

    board.gpios[IO_LEDCON6].io  = GPIOA;
	board.gpios[IO_LEDCON6].pin = GPIO_PIN_15;
	GpioInit(&(board.gpios[IO_LEDCON6]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_LEDCON6].io,board.gpios[IO_LEDCON6].pin, GPIO_PIN_SET);

    board.gpios[IO_TESTLED].io  = GPIOA;
	board.gpios[IO_TESTLED].pin = GPIO_PIN_4;
	GpioInit(&(board.gpios[IO_TESTLED]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(board.gpios[IO_TESTLED].io,board.gpios[IO_TESTLED].pin, GPIO_PIN_SET);
    
    board.gpios[IO_CNV_ADC1].io  = GPIOB;
	board.gpios[IO_CNV_ADC1].pin = GPIO_PIN_0;
	GpioInit(&(board.gpios[IO_CNV_ADC1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
   
    board.gpios[IO_CNV_ADC2].io  = GPIOB;
	board.gpios[IO_CNV_ADC2].pin = GPIO_PIN_1;
	GpioInit(&(board.gpios[IO_CNV_ADC2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
        
    board.gpios[IO_SPI_MOSI].io  = GPIOA;
	board.gpios[IO_SPI_MOSI].pin = GPIO_PIN_7;//spi's MOSI,use for AD7689           
    
}

/********************************************************************
 *
 *******************************************************************/
VOID  BoardInit(VOID)
{
	HAL_Init();
	RccInit();
    
	BoardGpioInit ();
	board.uartHost    = UartGetObject (UART02);
	board.uartConsole = UartGetObject (UART01);
	board.spiExt      = SpiGetObject  (SPI01);
    board.spiExt->dataSize = SPI_DATASIZE_16BIT;
	board.spiExt->clkPolarity = SPI_POLARITY_LOW;
	board.spiExt->clkPhase = SPI_PHASE_1EDGE;
    board.spiExt->spiDef.Init.NSSPMode=SPI_NSS_PULSE_DISABLE;
    SpiInit(BoardGetSpiExt());
    
}
/********************************************************************
 * End of File
 *******************************************************************/

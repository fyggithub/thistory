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
UINT16 MonitorGpio(UINT8 i)
{
	UINT16 t = (UINT16)(0x0001<<i);
	return t;
}

/********************************************************************
 *
 *******************************************************************/
typedef struct __Board{
	Uart* uartConsole;
	Uart* uartHost;
	Gpio  gpios[IO_MAX];
	Spi*  spiFRam;
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
Gpio* BoardGetGpio(UINT16 id)
{
	if (id < IO_MAX)
		return &(board.gpios[id]);
	return NULL;
}
/********************************************************************
 *
 *******************************************************************/
Spi*  BoardGetSpiFRam(VOID)
{
	return board.spiFRam;
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

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/********************************************************************
 *
 *******************************************************************/
VOID BoardGpioInit(VOID)
{
	for(UINT8 i=0; i<8; i++){
		board.gpios[i+IO_SAM_OPTO1].io  = GPIOG;
		board.gpios[i+IO_SAM_OPTO1].pin = MonitorGpio(i);		
		GpioInit(&(board.gpios[i+IO_SAM_OPTO1]),
			GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
	}
	
	for(UINT8 i=0; i<2; i++){
		board.gpios[i+IO_LED1].io  = GPIOG;
		board.gpios[i+IO_LED1].pin = MonitorGpio(i+11);		
		GpioInit(&(board.gpios[i+IO_LED1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);	
	}
		
	for(UINT8 i=0; i<8; i++){
		board.gpios[i+IO_SAM_LED1].io  = GPIOD;
		board.gpios[i+IO_SAM_LED1].pin = MonitorGpio(i);		
		GpioInit(&(board.gpios[i+IO_SAM_LED1]),
			GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	}
	
	for(UINT8 i=0; i<1; i++){
		board.gpios[i+IO_EMERGENCY_KEY].io  = GPIOE;
		board.gpios[i+IO_EMERGENCY_KEY].pin = MonitorGpio(10+i);		
		GpioInit(&(board.gpios[i+IO_EMERGENCY_KEY]),
			GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
	}
	
	for(UINT8 i=0; i<1; i++){
		board.gpios[i+IO_DOOR_OPTO].io  = GPIOF;
		board.gpios[i+IO_DOOR_OPTO].pin = MonitorGpio(i);		
		GpioInit(&(board.gpios[i+IO_DOOR_OPTO]),
			GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
	}


}
/********************************************************************
 *
 *******************************************************************/
VOID  BoardInit(VOID)
{
	HAL_Init();
	RccInit();

	BoardGpioInit ();
	board.uartHost    = UartGetObject (UART01);
	board.uartConsole = UartGetObject (UART03);	
}

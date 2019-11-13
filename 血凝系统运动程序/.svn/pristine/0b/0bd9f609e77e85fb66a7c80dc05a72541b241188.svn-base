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
	Spi*  spiFRam;
	Spi*  spiExt;
	Pulse* pulseMotor[MOTOR_MAX];
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
 *
 *******************************************************************/
Pulse*  BoardGetPulse(UINT16 motor_id)
{
	return board.pulseMotor[motor_id];
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
STATIC VOID BoardGpioInit(VOID)
{
	board.gpios[IO_LPD1].io  = GPIOF;
	board.gpios[IO_LPD1].pin = GPIO_PIN_14;
	GpioInit(&(board.gpios[IO_LPD1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_LPD2].io  = GPIOE;
	board.gpios[IO_LPD2].pin = GPIO_PIN_8;
	GpioInit(&(board.gpios[IO_LPD2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_LPD3].io  = GPIOE;
	board.gpios[IO_LPD3].pin = GPIO_PIN_10;
	GpioInit(&(board.gpios[IO_LPD3]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_LPD4].io  = GPIOE;
	board.gpios[IO_LPD4].pin = GPIO_PIN_12;
	GpioInit(&(board.gpios[IO_LPD4]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_ORI1].io  = GPIOF;
	board.gpios[IO_ORI1].pin = GPIO_PIN_15;
	GpioInit(&(board.gpios[IO_ORI1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_ORI2].io  = GPIOE;
	board.gpios[IO_ORI2].pin = GPIO_PIN_9;
	GpioInit(&(board.gpios[IO_ORI2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_ORI3].io  = GPIOE;
	board.gpios[IO_ORI3].pin = GPIO_PIN_11;
	GpioInit(&(board.gpios[IO_ORI3]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_ORI4].io  = GPIOE;
	board.gpios[IO_ORI4].pin = GPIO_PIN_13;
	GpioInit(&(board.gpios[IO_ORI4]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO1].io  = GPIOF;
	board.gpios[IO_OPTO1].pin = GPIO_PIN_0;
	GpioInit(&(board.gpios[IO_OPTO1]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO2].io  = GPIOF;
	board.gpios[IO_OPTO2].pin = GPIO_PIN_1;
	GpioInit(&(board.gpios[IO_OPTO2]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
		
	board.gpios[IO_OPTO3].io  = GPIOF;
	board.gpios[IO_OPTO3].pin = GPIO_PIN_2;
	GpioInit(&(board.gpios[IO_OPTO3]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
		
	board.gpios[IO_OPTO4].io  = GPIOF;
	board.gpios[IO_OPTO4].pin = GPIO_PIN_3;
	GpioInit(&(board.gpios[IO_OPTO4]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
		
	board.gpios[IO_PUMP1].io  = GPIOC;
	board.gpios[IO_PUMP1].pin = GPIO_PIN_2;
	GpioInit(&(board.gpios[IO_PUMP1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_PUMP2].io  = GPIOC;
	board.gpios[IO_PUMP2].pin = GPIO_PIN_3;
	GpioInit(&(board.gpios[IO_PUMP2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_VALVE1].io  = GPIOC;
	board.gpios[IO_VALVE1].pin = GPIO_PIN_4;
	GpioInit(&(board.gpios[IO_VALVE1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_LIQUID_COLL].io  = GPIOG;
	board.gpios[IO_LIQUID_COLL].pin = GPIO_PIN_8;
	GpioInit(&(board.gpios[IO_LIQUID_COLL]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_LIQUIDE_OUTPUT].io  = GPIOG;
	board.gpios[IO_LIQUIDE_OUTPUT].pin = GPIO_PIN_3;
	GpioHighInit(&(board.gpios[IO_LIQUIDE_OUTPUT]),//关闭校验，设置成高电平
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	
	board.gpios[IO_LIQUID_INPUT].io  = GPIOG;
	board.gpios[IO_LIQUID_INPUT].pin = GPIO_PIN_7;
	GpioExitInit(&(board.gpios[IO_LIQUID_INPUT]), 
				GPIO_MODE_IT_FALLING, GPIO_NOPULL,EXTI9_5_IRQn);
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
	board.spiFRam     = SpiGetObject (SPI02);
	board.spiExt      = SpiGetObject (SPI01);
	board.pulseMotor[0] = PulseGetObject (PULSE01);
	board.pulseMotor[1] = PulseGetObject (PULSE02);
	board.pulseMotor[2] = PulseGetObject (PULSE03);
	board.pulseMotor[3] = PulseGetObject (PULSE04);
}

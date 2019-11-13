/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Spi.c
 *******************************************************************/
#include "Spi.h"
#include "Console.h"
/********************************************************************
 *
 *******************************************************************/
#define SPI01_GPIO		GPIOA
#define SPI01_PIN_NSS	GPIO_PIN_4
#define SPI01_PIN_SCK	GPIO_PIN_5
#define SPI01_PIN_MISO	GPIO_PIN_6
#define SPI01_PIN_MOSI	GPIO_PIN_7
#define SPI01_SPI		SPI1
#define SPI01_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE ()
#define SPI01_SPI_CLK_ENABLE()		__HAL_RCC_SPI1_CLK_ENABLE ()

#define SPI02_GPIO		GPIOB
#define SPI02_PIN_NSS	GPIO_PIN_12
#define SPI02_PIN_SCK	GPIO_PIN_13
#define SPI02_PIN_MISO	GPIO_PIN_14
#define SPI02_PIN_MOSI	GPIO_PIN_15
#define SPI02_SPI		SPI2
#define GPIO_AF0_SPI01  GPIO_AF0_SPI2
#define SPI02_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE ()
#define SPI02_SPI_CLK_ENABLE()		__HAL_RCC_SPI2_CLK_ENABLE ()

#ifdef STM32F051x8
#define SPI01_ALTERNATE  GPIO_AF0_SPI1
#define SPI02_ALTERNATE  GPIO_AF0_SPI2
#define SPI_ALTERNATE(SPIID) do{gpioDef.Alternate = SPIID##_ALTERNATE;}while(0)
#else
#define SPI_ALTERNATE(SPIID)
#endif

/********************************************************************
 *
 *******************************************************************/
#define SPI_GPIO_INIT(SPIID)\
	do{\
		GPIO_InitTypeDef gpioDef;\
		SPIID##_GPIO_CLK_ENABLE ();\
		gpioDef.Pin   = SPIID##_PIN_SCK|SPIID##_PIN_MOSI;\
		gpioDef.Mode  = GPIO_MODE_AF_PP;\
		gpioDef.Speed = GPIO_SPEED_FREQ_HIGH;\
		gpioDef.Pull  = GPIO_NOPULL;\
		SPI_ALTERNATE(SPIID);\
		HAL_GPIO_Init (SPIID##_GPIO, &gpioDef);\
		\
		gpioDef.Pin   = SPIID##_PIN_MISO;\
		gpioDef.Mode  = GPIO_MODE_AF_PP;\
		gpioDef.Pull  = GPIO_NOPULL;\
		gpioDef.Speed = GPIO_SPEED_FREQ_HIGH;\
		SPI_ALTERNATE(SPIID);\
		HAL_GPIO_Init (SPIID##_GPIO, &gpioDef);\
	}while(0)
/********************************************************************
 *
 *******************************************************************/
#define SPI_INIT(SPIID, spi)\
	do{\
		SPI_HandleTypeDef* inst = &((spi)->spiDef);\
		SPIID##_SPI_CLK_ENABLE ();\
		inst->Instance         = SPIID##_SPI;\
		inst->Init.Mode        = SPI_MODE_MASTER;\
		inst->Init.Direction   = SPI_DIRECTION_2LINES;\
		inst->Init.DataSize    = (spi)->dataSize;\
		inst->Init.CLKPolarity = (spi)->clkPolarity;\
		inst->Init.CLKPhase    = (spi)->clkPhase;\
		inst->Init.NSS         = SPI_NSS_SOFT;\
		inst->Init.BaudRatePrescaler \
		                       = (spi)->bdrPrescaler;\
		inst->Init.FirstBit    = (spi)->firstBit;\
		inst->Init.TIMode            = SPI_TIMODE_DISABLE;\
		inst->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;\
		/*inst->Init.CRCPolynomial     = 10;*/\
  		HAL_SPI_Init(inst);\
	}while(0)

/********************************************************************
 *
 *******************************************************************/

STATIC Spi spis[SPI_MAX];
STATIC GPIO_TypeDef* spiCSIO[SPI_MAX] = {
	SPI01_GPIO, SPI02_GPIO,
};
STATIC UINT16 spiCSPIN[] = {
	SPI01_PIN_NSS, SPI02_PIN_NSS,
};

/********************************************************************
 *
 *******************************************************************/
VOID   SpiInit(Spi* spi)
{
	if (spi) {
		if (spi->id == SPI01){
			SPI_GPIO_INIT (SPI01);
			SPI_INIT (SPI01, spi);
		}
		if (spi->id == SPI02) {
			SPI_GPIO_INIT (SPI02);
			SPI_INIT (SPI02, spi);
		}
		GpioInit(&(spi ->csPin), GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
		SpiDisableCS (spi);
	}
}

/********************************************************************
 *
 *******************************************************************/
VOID SpiEnableCS(Spi* spi)
{
	if (spi) {
		GpioSet (&(spi->csPin), 0);
	}
}
VOID SpiDisableCS(Spi* spi)
{
	if (spi) {
		GpioSet (&(spi->csPin), 1);
	}
}

/********************************************************************
 *
 *******************************************************************/
/*
UINT16 SpiSendRecv(Spi* spi, BYTE* pTxData,BYTE* pRxData, UINT16 len)
{
    if (spi && pTxData && pRxData && len>0){
		SPI_HandleTypeDef * hdl = &(spi->spiDef);
#ifndef STM32F051x8
		SpiEnableCS (spi);
#endif
        HAL_StatusTypeDef ret =
            HAL_SPI_TransmitReceive(hdl, pTxData, pRxData, len, 0xFFFF);
        if (ret == HAL_OK)
            return len;
    }
	return 0;
}
*/
/********************************************************************
 *
 *******************************************************************/
UINT16 SpiSend(Spi* spi, BYTE* data, UINT16 len)
{
	if (spi && data && len>0){
		SPI_HandleTypeDef * hdl = &(spi->spiDef);
		HAL_StatusTypeDef ret =
			HAL_SPI_Transmit (hdl, data, len, 0xFFFF);
		if (ret == HAL_OK)
			return len;
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
UINT16 SpiRecv(Spi* spi, BYTE* data, UINT16 len)
{
	if (spi && data && len>0) {
		SPI_HandleTypeDef * hdl = &(spi->spiDef);
		HAL_StatusTypeDef ret =
			HAL_SPI_Receive (hdl, data, len, 0xFFFF);
		if (ret == HAL_OK){
			return len;
		}
	}
	return 0;
}
/********************************************************************
 *
 *******************************************************************/
Spi*   SpiGetObject(UINT16 id)
{
	if (id<SPI_MAX) { // return default settings
		spis[id].id = id;
		spis[id].bdrPrescaler = SPI_BAUDRATEPRESCALER_256;
		spis[id].firstBit     = SPI_FIRSTBIT_MSB;
		spis[id].clkPolarity  = SPI_POLARITY_HIGH; // CPOL = 1
		spis[id].clkPhase     = SPI_PHASE_2EDGE;   // CPHA = 1
		spis[id].dataSize     = SPI_DATASIZE_8BIT;
		spis[id].csPin.io  = spiCSIO [id];
		spis[id].csPin.pin = spiCSPIN[id];
		return &(spis[id]);
	}
	return NULL;
}
/********************************************************************
 * End of File
 *******************************************************************/



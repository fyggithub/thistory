/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Adc.c
 *******************************************************************/
#include "Adc.h"
#include "Gpio.h"
#include "Board.h"
struct _tempRem{
    DOUBLE temp3Mid[3];
    UINT16 temp3MidId;
    DOUBLE temp5Mean[5];
    UINT16 temp5MeanId;
}tempRem;
/********************************************************************
 *
 *******************************************************************/
ADC_HandleTypeDef hadcx;
UINT32 AdcValue[3];
/********************************************************************
 *
 *******************************************************************/
VOID MX_ADCx_Init(VOID)
{
    ADC_InjectionConfTypeDef sConfigInjected;

    hadcx.Instance = ADC1;
    hadcx.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadcx.Init.ContinuousConvMode = DISABLE;
    hadcx.Init.DiscontinuousConvMode = DISABLE;
    hadcx.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadcx.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadcx);
    
    sConfigInjected.InjectedChannel = ADC_CHANNEL_4;
    sConfigInjected.InjectedRank = 1;
    sConfigInjected.InjectedNbrOfConversion = 3;
    sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
    sConfigInjected.AutoInjectedConv = DISABLE;
    sConfigInjected.InjectedDiscontinuousConvMode = ENABLE;
    sConfigInjected.InjectedOffset = 0;
    HAL_ADCEx_InjectedConfigChannel(&hadcx, &sConfigInjected);
  
    sConfigInjected.InjectedChannel = ADC_CHANNEL_5;
    sConfigInjected.InjectedRank = 2;
    HAL_ADCEx_InjectedConfigChannel(&hadcx, &sConfigInjected);

    sConfigInjected.InjectedChannel = ADC_CHANNEL_6;
    sConfigInjected.InjectedRank = 3;
    HAL_ADCEx_InjectedConfigChannel(&hadcx, &sConfigInjected);
}
/********************************************************************
 *
 *******************************************************************/
VOID HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hadc->Instance==ADC1){
        __HAL_RCC_ADC1_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    }
}
/********************************************************************
 *
 *******************************************************************/
VOID HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    for(UINT8 i=0; i<3; i++){
        AdcValue[i]=HAL_ADCEx_InjectedGetValue(&hadcx, i+1);
    }   
}
/********************************************************************
 *
 *******************************************************************/
VOID ADC1_2_IRQHandler(VOID)
{
    HAL_ADC_IRQHandler(&hadcx);
}
/********************************************************************
 *
 *******************************************************************/
VOID AdcStart(VOID)
{
    AdcValue[0]=0;
    AdcValue[1]=0;
    AdcValue[2]=0;

    MX_ADCx_Init(); 
    HAL_ADCEx_Calibration_Start(&hadcx);
    HAL_ADCEx_InjectedStart_IT(&hadcx);
}

/********************************************************************
 * End of File
 *******************************************************************/

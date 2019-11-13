/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 串口IAP功能实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "led/bsp_led.h"
#include "stmflash/stm_flash.h"
#include "IAP/bsp_iap.h"
#include "key/bsp_key.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t Rxdata;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9倍频，得到72MHz主时钟
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1时钟：36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{  
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();

  LED_GPIO_Init();
  KEY_GPIO_Init();
  
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();
  HAL_UART_Receive_IT(&husart_debug,&Rxdata,1);
  
#ifdef User_Flash
  printf("下面将烧写 APP 到 FLASH\n");
  printf("如果需要更新 APP，请发送 APP 固件（.bin文件），完毕后请单击 KEY1。否则单击 KEY2 执行 APP。\n");
  
  /* 无限循环 */
  while(1)                            
  {	 
    if(KEY1_StateRead()==KEY_DOWN)             //如果KEY1被单击
    {
      printf("APP 长度：%d字节\n", strAppBin .usLength );				
      printf("开始更新 APP\n");
      //更新FLASH代码  	
      IAP_Write_App_Bin(APP_START_ADDR, strAppBin .ucDataBuf, strAppBin .usLength );
      printf ( "单击 KEY2，执行 APP\n" );
    }			
    if(KEY2_StateRead()==KEY_DOWN)             //如果KEY2被单击
    {	
      printf ( "开始执行 APP\n" ); 
      //执行FLASH APP代码
      IAP_ExecuteApp(APP_START_ADDR);
    }
	}    
#else		
  printf("下面将烧写 APP 到 RAM\n");
  printf("请发送 APP 固件（.bin文件）\n");	
  printf("发送完毕后请单击 KEY2 执行 APP\n");			
  while(1)                            
  {	   
    if(KEY2_StateRead()==KEY_DOWN )             //如果KEY2被单击
    {
      printf("APP 长度：%d字节\n",strAppBin .usLength);					
      printf("开始执行 APP\n"); 					
      IAP_ExecuteApp ( APP_START_ADDR );    //执行RAM APP代码
    }
  }
#endif
}

/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(strAppBin.usLength<APP_FLASH_LEN)
  {
		strAppBin.ucDataBuf[strAppBin.usLength++]=Rxdata;
  }
  HAL_UART_Receive_IT(&husart_debug,&Rxdata,1);
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

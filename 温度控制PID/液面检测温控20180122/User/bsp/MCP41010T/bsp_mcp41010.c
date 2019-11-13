#include "bsp/MCP41010T/bsp_mcp41010.h"

void MCP41010_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		 
	RCC_APB2PeriphClockCmd(MCP41010_RCC_CSGPIO|MCP41010_RCC_CLKGPIO |MCP41010_RCC_SIGPIO, ENABLE); /* ʹ��(����)���Ŷ�ӦIO�˿�ʱ�� */ 

	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CS_PIN;  	    /* �趨LED1��Ӧ����IO��� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */ 	
	GPIO_Init(MCP41010_CS_GPIO, &GPIO_InitStructure);   /* ��ʼ��LED1��Ӧ����IO */
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CLK_PIN;  	    /* �趨LED1��Ӧ����IO��� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */ 	
	GPIO_Init(MCP41010_CLK_GPIO, &GPIO_InitStructure);   /* ��ʼ��LED1��Ӧ����IO */
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_SI_PIN;  	    /* �趨LED1��Ӧ����IO��� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */ 	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(MCP41010_SI_GPIO, &GPIO_InitStructure);   /* ��ʼ��LED1��Ӧ����IO */
	
	MCP41010_CS_ON;
	MCP41010_CLK_OFF;
	MCP41010_SI_OFF;
}

void MCP41010_Write(uint16_t write_data)
{
	uint16_t i = 0,data_value = 0x11;
	
	MCP41010_CS_OFF;
	delay_nop(1);
	MCP41010_CLK_OFF;	
	delay_nop(1);
//	MCP41010_SI_OFF;
	
	for(i = 0;i < 8;i++)
	{		
		
		if((data_value & 0x80) == 0)
			MCP41010_SI_OFF;
		else		
			MCP41010_SI_ON;
		delay_nop(1);
		MCP41010_CLK_ON;
		delay_nop(1);
		MCP41010_CLK_OFF;
		delay_nop(1);	
data_value <<= 1;		
	}
	for(i = 0;i < 8;i++)
	{		
		
		if((write_data & 0x80) == 0)
			MCP41010_SI_OFF;
		else		
			MCP41010_SI_ON;

		delay_nop(1);
		MCP41010_CLK_ON;
		delay_nop(1);
		MCP41010_CLK_OFF;
		delay_nop(1);
			write_data <<= 1;
	}	
	MCP41010_CS_ON;
}

void delay_nop(uint16_t num_nop)
{
	uint16_t i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < num_nop;j++);
	}
}




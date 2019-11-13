#ifndef __BSP_DEALPID_H__
#define __BSP_DEALPID_H__


#include "stm32f10x.h"
#include "bsp/PID/bsp_pid.h"
#include "bsp/GeneralTIM/bsp_GeneralTIM.h" 
#include "bsp/USART/bsp_usart.h"
#include "stdlib.h"
#include "math.h"
#include "bsp/MCP41010T/bsp_mcp41010.h"

#define ROOM_TEMPERATUER_RES     10740.0     //³£ÎÂµç×èÎª10.74K
#define RES_PARAMETER_B          3450      //ÈÈÃôµç×èµÄ²ÎÊýBÎ3450            
#define CALVIN_PARAMETER         273.15
#define CALVIN_TEMPERATUER       1/(CALVIN_PARAMETER + 25)
#define VALTAGE_GAIN             3.12766

#define RES_12K            12000
#define RES_8K             8000
#define EXCEED_12KRES_A    -1.7629
#define EXCEED_12KRES_B    43.127
#define RANGE8_12KRES_A    -2.7933
#define RANGE8_12KRES_B    54.848
#define BELOW_8KRES_A      -4.0471
#define BELOW_8KRES_B      64.863

#define MAXPWM       0.7
#define MINPWM       0

#define ERROR_NUM    0
#define KP_NUM       7
#define KI_NUM       14
#define KD_NUM       21
#define PIDSUM_NUM   28
#define PIDOUT_NUM   36
#define PWM_NUM      44
#define TEMP_NUM     51

typedef struct
{
	uint8_t Sum_Buff[100];
	
	uint16_t P_Buff[10];
	uint16_t I_Buff[10];
	uint16_t D_Buff[10];
	
	uint32_t P_SumValue;
	uint32_t I_SumValue;
	uint32_t D_SumValue;
}PID_Data;

typedef struct
{
	uint16_t channelvalue[3];
	float mearsuing_resvalue;
	float voltagevalue;
	float tmepvalue;
	float adjustvalue;
	float detectvalue;
}ADC_Data;

typedef struct
{
	uint16_t timeup_flag;
}ADC_Management;

typedef struct
{
	uint16_t count_value;
}PWM_Management;

typedef struct
{
	float error_value;
	float kp_value;
	float ki_value;
	float kd_value;
	float pidlastsum;
	float pidsum;
	float pidafterout;
	float pidout;	
	float pwm_value;
}Dis_PID;

void Set_Pwm(int16_t set_value);
void Read_AdcValue(void);
void Display_AdcValue(void);
void Deal_Pid(void);
void Deal_ReadValue(void);
uint16_t Numerical_Wave(uint16_t *p,uint16_t num);
void Bubble_Sort(uint16_t *p,uint16_t num);


extern Dis_PID Dis_Value;
extern PID_Data PID_Parameter;
extern ADC_Data ADC_Parameter;
extern ADC_Management ADC_Stat;
extern PWM_Management PWM_Pameter;

#endif



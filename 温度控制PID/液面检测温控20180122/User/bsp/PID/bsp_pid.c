
/*********************************************************************************************************
*
* File                : bsp_pid.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 5.0
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2017 Ike's Lab
*                                        All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp/PID/bsp_pid.h"

PID_TypeDef temperature_pid;

void pid_init(void)
{
    temperature_pid.mode = PID_Pos_Mode;   //使用位置式
    temperature_pid.SetPoint = 37.0;
    temperature_pid.Proportion = 0.6;
    temperature_pid.Integral = 0.0005;
    temperature_pid.Derivative = 0;
    temperature_pid.LastError = 0;
    temperature_pid.PrevError = 0;
	
	PID_Parameter.P_SumValue = temperature_pid.Proportion * 100;
	PID_Parameter.I_SumValue = temperature_pid.Integral * 100;
	PID_Parameter.D_SumValue = temperature_pid.Derivative * 100;
}

float PID_Calc(PID_TypeDef* PID, float NextPoint ) 
{ 
	float Errormmm,temp ;

	if(PID->mode == PID_Inc_Mode)   			//增量式
	{
		Errormmm = (PID->SetPoint * TemperatureRisitance_Linearity) - NextPoint;        		
		temp =	 
			PID->Proportion*(Errormmm-PID->LastError) +
			PID->Integral*Errormmm +
			PID->Derivative*(Errormmm- 2*PID->LastError+ PID->PrevError);
		
		PID->PrevError = PID->LastError; 
		PID->LastError = (int)Errormmm;		
		return temp; 
	}
	else           //位置式
	{		  
		Errormmm = (PID->SetPoint * TemperatureRisitance_Linearity) - NextPoint;        // 位置式
//		PID->SumError += Errormmm; 
		
		Dis_Value.error_value = Errormmm;
		Dis_Value.kp_value = PID->Proportion*Errormmm;
		
		if( (Errormmm >= -0.8 ) && (Errormmm <= 0.8))                        //整个过程的积分累加，绝对值超过0.5就清零，不足就累加
			Dis_Value.ki_value += PID->Integral * Errormmm * FACTOR;    //加上上次的值
		else
			Dis_Value.ki_value = 0;

		Dis_Value.kd_value = PID->Derivative*(Errormmm-PID->LastError);
		
		temp = Dis_Value.kp_value + Dis_Value.ki_value + Dis_Value.kd_value;
	
		PID->LastError = (int)Errormmm;  
		return temp; 
	}
} 





/******************* (C) COPYRIGHT 2014 Ike's Lab *****END OF FILE****/

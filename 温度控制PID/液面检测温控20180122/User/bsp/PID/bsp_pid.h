
/*********************************************************************************************************
*
* File                : pid.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.54
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2014 Ike's Lab
*                                        All Rights Reserved
*
*********************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PID_H__
#define __BSP_PID_H__ 			   

/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------- */

/* Private typedef ------------------------------------------------------- */

#include "bsp/DealPID/bsp_dealpid.h"
#include "stdlib.h"
#include "math.h"

#define TemperatureRisitance_Linearity 1
#define SEPARATION  0.5
#define FACTOR      1  

typedef enum
{
	PID_Inc_Mode = 0, 
	PID_Pos_Mode = 1
}PID_Calc_Mode;

typedef struct 
{
	PID_Calc_Mode mode; //???,???
	
	float SetPoint; //???? Desired Value
	float Actual;
	long CalculateActual;   //add
	
	float Proportion; //???? Proportional Const 
	float Integral; //???? Integral Const 
	float Derivative; //???? Derivative Const
	
	long SumError; //???? 
	
	int LastError; //Error[-1] 
	int PrevError; //Error[-2]
	
} PID_TypeDef;


/* Exported functions ------------------------------------------------------- */
void pid_init(void);
float PID_Calc(PID_TypeDef* PID, float NextPoint ) ; 

extern PID_TypeDef temperature_pid;

#endif

/******************* (C) COPYRIGHT 2014 Ike's Lab *****END OF FILE****/

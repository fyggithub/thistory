#include "bsp/DealPID/bsp_dealpid.h"

Dis_PID Dis_Value;
PID_Data PID_Parameter;
ADC_Data ADC_Parameter;
ADC_Management ADC_Stat;
PWM_Management PWM_Pameter;

void Deal_Pid(void)
{
	Read_AdcValue();
	
	Dis_Value.pidsum  = PID_Calc(&temperature_pid,ADC_Parameter.tmepvalue);
	Dis_Value.pidout = Dis_Value.pidlastsum * 0.7 + Dis_Value.pidsum * 0.3;   //滤波一次
	Dis_Value.pidafterout = Dis_Value.pidout;    //主要为了显示此时的值
	Dis_Value.pidlastsum = Dis_Value.pidout;   //记录上次的PIDout值
	
	if(Dis_Value.pidout > MAXPWM)     //para:限幅	
		Dis_Value.pidout = MAXPWM;	
	else if(Dis_Value.pidout < MINPWM)
		Dis_Value.pidout = MINPWM;
	
	Dis_Value.pwm_value = Dis_Value.pidout;
	Set_Pwm(Dis_Value.pwm_value * 100);
	
	Display_AdcValue();   //输出日志
}

void Read_AdcValue(void)
{
	float lnfuction_value = 0.0,cal_num = 0.0;
	
	ADC_Parameter.voltagevalue  = (float)ADC_Parameter.channelvalue[0] * 3.3 / 4096;    //读取ADC中读取的值,转化成电压
	ADC_Parameter.adjustvalue = (float)ADC_Parameter.channelvalue[1] * 3.3 / 4096;
	ADC_Parameter.detectvalue = (float)ADC_Parameter.channelvalue[2] * 3.3 / 4096;
	ADC_Parameter.mearsuing_resvalue = 12000 - ADC_Parameter.voltagevalue * 10000 / VALTAGE_GAIN;
	
	lnfuction_value = log(ADC_Parameter.mearsuing_resvalue/ROOM_TEMPERATUER_RES) ;
	cal_num = lnfuction_value / RES_PARAMETER_B + CALVIN_TEMPERATUER ;
	ADC_Parameter.tmepvalue = 1.0 / cal_num - CALVIN_PARAMETER; 				 //将电压转化成温度值
	
//	if(ADC_Parameter.mearsuing_resvalue > RES_12K)   //模拟计算的温度
//	{
//		ADC_Parameter.tmepvalue = ADC_Parameter.mearsuing_resvalue / 1000.0 * EXCEED_12KRES_A + EXCEED_12KRES_B;
//	}
//	else if( (ADC_Parameter.mearsuing_resvalue <= RES_12K) && (ADC_Parameter.mearsuing_resvalue >= RES_8K))
//	{
//		ADC_Parameter.tmepvalue = ADC_Parameter.mearsuing_resvalue / 1000.0 * RANGE8_12KRES_A + RANGE8_12KRES_B;
//	}
//	else
//	{
//		ADC_Parameter.tmepvalue = ADC_Parameter.mearsuing_resvalue / 1000.0 * BELOW_8KRES_A + BELOW_8KRES_B;
//	}	
//	MCP41010_Write(0x55);
}

void Set_Pwm(int16_t set_value)
{
	PWM_Pameter.count_value = set_value;
}

void Display_AdcValue(void)
{
	static long send_error,send_kp,send_ki,send_kd,send_pidsum,send_pidout,send_pwm,send_tmepvalue;

/***********************发送目标与实际测量的差值**********************/	
	send_error = Dis_Value.error_value * 100;
	if(send_error >= 0)
	{
		dailyrecord_buff[0] = '+';
	}		
	else
	{
		send_error = send_error * (-1);
		dailyrecord_buff[0] = '-';
	}		
	dailyrecord_buff[1] = send_error / 1000 + 0x30;
	dailyrecord_buff[2] = send_error % 1000 / 100 + 0x30;
	dailyrecord_buff[3] = '.';
	dailyrecord_buff[4] = send_error % 100 / 10 + 0x30;
	dailyrecord_buff[5] = send_error % 10 + 0x30;
	dailyrecord_buff[6] = '\t';

/***********************发送比例项**********************/		
	send_kp = Dis_Value.kp_value * 100;
	if(send_kp >= 0)
	{
		dailyrecord_buff[7] = '+';
	}
	else
	{
		send_kp = send_kp * (-1);
		dailyrecord_buff[7] = '-';
	}
	dailyrecord_buff[8] = send_kp / 1000 + 0x30;
	dailyrecord_buff[9] = send_kp % 1000 / 100 + 0x30;
	dailyrecord_buff[10] = '.';
	dailyrecord_buff[11] = send_kp % 100 / 10 + 0x30;
	dailyrecord_buff[12] = send_kp % 10 + 0x30;
	dailyrecord_buff[13] =  '\t';

/***********************发送积分项**********************/	
	send_ki = Dis_Value.ki_value * 10000;
	if(send_ki >= 0)
	{
		dailyrecord_buff[14] = '+';
	}
	else
	{
		send_ki = send_ki * (-1);
		dailyrecord_buff[14] = '-';
	}
	dailyrecord_buff[15] = send_ki / 100000 % 10000 + 0x30;
	dailyrecord_buff[16] = send_ki / 10000 % 1000 + 0x30;
	dailyrecord_buff[17] = '.';
	dailyrecord_buff[18] = send_ki / 1000 % 100 + 0x30;
	dailyrecord_buff[19] = send_ki % 1000 / 100 + 0x30;	
	dailyrecord_buff[20] = send_ki % 100 / 10 + 0x30;
	dailyrecord_buff[21] = send_ki % 10 + 0x30;
	dailyrecord_buff[22] = '\t';

/***********************发送微分项**********************/	
	send_kd = Dis_Value.kd_value * 100;
	if(send_kd >= 0)
	{
		dailyrecord_buff[23] = '+';
	}
	else
	{
		send_kd = send_kd * (-1);
		dailyrecord_buff[23] = '-';
	}
	dailyrecord_buff[24] = send_kd / 1000 + 0x30;
	dailyrecord_buff[25] = send_kd % 1000 / 100 + 0x30;
	dailyrecord_buff[26] = '.';
	dailyrecord_buff[27] = send_kd % 100 / 10 + 0x30;
	dailyrecord_buff[28] = send_kd % 10 + 0x30;
	dailyrecord_buff[29] = '\t';
	
/***********************发送PIDsum**********************/
	send_pidsum = Dis_Value.pidsum * 100;
	if(send_pidsum >= 0)
	{
		dailyrecord_buff[30] = '+';
	}
	else
	{
		send_pidsum = send_pidsum * (-1);
		dailyrecord_buff[30] = '-';
	}
	dailyrecord_buff[31] = send_pidsum / 10000 + 0x30;
	dailyrecord_buff[32] = send_pidsum % 10000 / 1000 + 0x30;
	dailyrecord_buff[33] = send_pidsum % 1000 / 100 + 0x30;
	dailyrecord_buff[34] = '.';
	dailyrecord_buff[35] = send_pidsum % 100 / 10 + 0x30;
	dailyrecord_buff[36] = send_pidsum % 10 + 0x30;
	dailyrecord_buff[37] = '\t';
	
/***********************发送PIDout**********************/
	send_pidout = Dis_Value.pidafterout * 100;
	if(send_pidout >= 0)
	{
		dailyrecord_buff[38] = '+';
	}
	else
	{
		send_pidout = send_pidout * (-1);
		dailyrecord_buff[38] = '-';
	}
	dailyrecord_buff[39] = send_pidout / 10000 + 0x30;
	dailyrecord_buff[40] = send_pidout % 10000 / 1000 + 0x30;
	dailyrecord_buff[41] = send_pidout % 1000 / 100 + 0x30;
	dailyrecord_buff[42] = '.';
	dailyrecord_buff[43] = send_pidout % 100 / 10 + 0x30;
	dailyrecord_buff[44] = send_pidout % 10 + 0x30;
	dailyrecord_buff[45] = '\t';
	
/***********************发送PWM************************/
	send_pwm = Dis_Value.pwm_value * 1000;
	dailyrecord_buff[46] = send_pwm / 1000 + 0x30;
	dailyrecord_buff[47] = send_pwm % 1000 / 100 + 0x30;	
	dailyrecord_buff[48] = send_pwm % 100 / 10 + 0x30;
	dailyrecord_buff[49] = '.';
	dailyrecord_buff[50] = send_pwm % 10 + 0x30;
	dailyrecord_buff[51] = '%';
	dailyrecord_buff[52] = '\t';

/***********************发送温度************************/	
	send_tmepvalue = ADC_Parameter.tmepvalue * 100;
	dailyrecord_buff[53] = send_tmepvalue / 1000 + 0x30;
	dailyrecord_buff[54] = send_tmepvalue % 1000 / 100 + 0x30;
	dailyrecord_buff[55] = '.';
	dailyrecord_buff[56] = send_tmepvalue % 100 / 10 + 0x30;
	dailyrecord_buff[57] = send_tmepvalue % 10 + 0x30;
	dailyrecord_buff[58] = '\n';
	
//	DMA_Cmd (USARTx_DMA_CHANNEL,ENABLE);
//	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
	
	Usart_SendStr_length(dailyrecord_buff,59);
}

void Deal_ReadValue(void)
{
	static uint16_t deal_count1 = 0,deal_count2 = 0,fristenter_flag = 0;
	static  uint16_t dealdata_buff1[12],dealdata_buff2[12];
	
	dealdata_buff1[deal_count1++] = ADC_Parameter.voltagevalue;	
	if(fristenter_flag == 0)   
	{
		dealdata_buff1[deal_count1++] = ADC_Parameter.channelvalue[0];
		if(deal_count1 >= 5)        //取前5次的平均值，附初值温度
		{
			deal_count1 = 0;
			fristenter_flag = 1;
			ADC_Parameter.channelvalue[0] = Numerical_Wave(dealdata_buff1,5);   //温度的初始值
		}
	}
	else
	{
		if(deal_count1 >= 3)
		{
			deal_count1 = 0;
			Bubble_Sort(dealdata_buff1,3);   //排好序，取出A的mean值（及中间值）
			
			dealdata_buff2[deal_count2++] = dealdata_buff1[1];  //存储中间值
			if(deal_count2 >= 5)
			{
				deal_count2 = 0;
				ADC_Parameter.channelvalue[0] = Numerical_Wave(dealdata_buff2,5);  //将B中的值的平均值传给即将要进行滤波
			}
		}
	}		

}

uint16_t Numerical_Wave(uint16_t *p,uint16_t num)
{
	uint16_t i = 0;
	uint16_t average_value = 0,sum_value = 0;
	
	for(i = 0;i < num;i++)
	{
		sum_value += *(p + i);
	}
	average_value = (uint16_t)(sum_value / num);
	
	return average_value;
}

void Bubble_Sort(uint16_t *p,uint16_t num)
{
	uint16_t i,j,temp;
	for(i = 0;i < num;i++)
	{
		for(j = 0;j < num - i -1;j++)
		{
			if(p[j] > p[j + 1])
			{
				temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
		}
	}
}

//void Display_AdcValue(void)
//{
//	static long send_voltagevalue,send_res,send_tmepvalue,send_adjustvalue,send_detectvalue;
//	
//	Read_AdcValue();
//	Set_Pwm(90);
//	
//	send_voltagevalue = ADC_Parameter.voltagevalue * 1000;
//	dailyrecord_buff[0] = send_voltagevalue / 1000 + 0x30;
//	dailyrecord_buff[1] = '.';
//	dailyrecord_buff[2] = send_voltagevalue % 1000 / 100 + 0x30;	
//	dailyrecord_buff[3] = send_voltagevalue % 100 / 10 + 0x30;
//	dailyrecord_buff[4] = send_voltagevalue % 10 + 0x30;
//	dailyrecord_buff[5] = '\t';
//	
//	send_res = ADC_Parameter.mearsuing_resvalue;
//	dailyrecord_buff[6] = send_res / 10000 + 0x30;
//	dailyrecord_buff[7] = send_res % 10000 / 1000 + 0x30;
//	dailyrecord_buff[8] = send_res % 1000 / 100 + 0x30;	
//	dailyrecord_buff[9] = send_res % 100 / 10 + 0x30;
//	dailyrecord_buff[10] = send_res % 10 + 0x30;
//	dailyrecord_buff[11] = '\t';
//	
//	send_tmepvalue = ADC_Parameter.tmepvalue * 100;
//	dailyrecord_buff[12] = send_tmepvalue / 1000 + 0x30;
//	dailyrecord_buff[13] = send_tmepvalue % 1000 / 100 + 0x30;
//	dailyrecord_buff[14] = '.';
//	dailyrecord_buff[15] = send_tmepvalue % 100 / 10 + 0x30;
//	dailyrecord_buff[16] = send_tmepvalue % 10 + 0x30;
//	dailyrecord_buff[17] = '\n';
//	
////	send_adjustvalue = ADC_Parameter.adjustvalue * 100;
////	dailyrecord_buff[18] = send_adjustvalue / 1000 + 0x30;
////	dailyrecord_buff[19] = '.';
////	dailyrecord_buff[20] = send_adjustvalue % 1000 / 100 + 0x30;	
////	dailyrecord_buff[21] = send_adjustvalue % 100 / 10 + 0x30;
////	dailyrecord_buff[22] = send_adjustvalue % 10 + 0x30;
////	dailyrecord_buff[23] = '\t';
////		
////	send_detectvalue = ADC_Parameter.detectvalue * 100;
////	dailyrecord_buff[24] = send_detectvalue / 1000 + 0x30;
////	dailyrecord_buff[25] = '.';
////	dailyrecord_buff[26] = send_detectvalue % 1000 / 100 + 0x30;	
////	dailyrecord_buff[27] = send_detectvalue % 100 / 10 + 0x30;
////	dailyrecord_buff[28] = send_detectvalue % 10 + 0x30;
////	dailyrecord_buff[29] = '\n';
//	
//	Usart_SendStr_length(dailyrecord_buff,18);
//	
////	DMA_Cmd (USARTx_DMA_CHANNEL,ENABLE);
////	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
//}
















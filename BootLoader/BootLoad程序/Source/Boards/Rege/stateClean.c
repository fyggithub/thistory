#include "stateClean.h"

/********************************************************************
 *������ϴʱԤ��PWM�Ĺ�ʽ
 *******************************************************************/
INT16 GetClnPwm(VOID)
{
	INT16 temp;
	temp = sqrt(0.03687 * K1 * (37 - Ta) / CLN_HEAT_TIME * 1000) * 1000;
	return temp;
}
/********************************************************************
 *�����Լ�ʱԤ��PWM�Ĺ�ʽ
 *******************************************************************/
INT16 GetSubPwm(VOID)
{
	INT16 temp;
	temp = sqrt(0.003675 * K2 * MainState.Parameter.subVolume / REGE_HEAT_TIME * 1000) * 1000;
	return temp;
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID InnerPumpOn(Message* msg)
{
//	STATIC UINT8 count = 0;
	INT16 time;
	if(GpioGet(BoardGetGpio(IO_PUMP1)) == GPIO_OFF)
	{
		if(msg->id == MSG_TIMER2){			
				time = OPEN_HEAT_TIME / 100;
				switch(MainState.Parameter.clnType)
				{
					case 1:TimerStart2(100, time);break;		//�ڱڱö�ʱ����						
					case 2:TimerStart2(100, time);break;			//�ڱڱö�ʱ����		
					default:break;
				}										
				GpioSet (BoardGetGpio(IO_PUMP1),GPIO_ON);				 //���ڱڱ�
		}
	}
	else
	{
		if(msg->id == MSG_TIMER2){
			if(MainState.Parameter.countpump == 0){
				MainState.Parameter.countpump++;
				switch(MainState.Parameter.clnType)
				{
					case 1:{
							time = ConfigGet(CFG_REAGENT_INNER_TIME) - 2 - OPEN_HEAT_TIME / 100;
							TimerStart2(100, time);
						}break;		//�ڱڱö�ʱ����						
					case 2:{
							time = MainState.Parameter.innerTime - 2 - OPEN_HEAT_TIME / 100;
							TimerStart2(100, time);				
						}break;			//�ڱڱö�ʱ����		
					default:break;
				}
				if(OPEN_CLN_HEAT)
					RegeSendSlaveCmdMode(GetClnPwm(),CLN_HEAT_TIME,CLN_STOP_TIME);   //����ָ�����Һ������Կ�ʼԤ��		
			}
			else
			{
				MainState.Parameter.countpump = 0;
				GpioSet (BoardGetGpio(IO_PUMP1),GPIO_OFF);	//�ر��ڱڱ�
			}
		}			
	}
}

STATIC VOID CleanProcess(Message* msg)
{
	InnerPumpOn(msg);   //�ڱڱõ�ʱ�����
	switch(MainState.stateManageBtm)
	{
		case STATE2_CLN_CLN_DRA:{
						if(msg->id == MSG_Z_DONE){
							switch(MainState.Parameter.clnType)
							{
								case 1:TimerStart(100, ConfigGet(CFG_REAGENT_OUTTER_TIME));break; //��ͨ��ϴ   //�ڱڷ�����ڱö�ʱ����//��Ҫ�Ƕ�ʱ��һ������ܶ�ʱ1200ms																			  									
								case 2:TimerStart(100, MainState.Parameter.outerTime);break; 		//����ʱ����ϴ						
								default:break;
							}
							TimerStart2(100, 1);		//��100ms�ڱڱô�								
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_REAGENT_SYRINGE_NEGLMT), REAGENT_ZAXISDRAINCLE_FALLSPEED);   //����													
							GpioSet (BoardGetGpio(IO_VALVE1),GPIO_ON);		//���ڱڷ�			
							GpioSet (BoardGetGpio(IO_PUMP2),GPIO_ON);		//����ڱ�																																																																	
							MainState.stateManageBtm = STATE2_CLN_CLN_SUB;
						}										
					}break;
		case STATE2_CLN_CLN_SUB:{									
						if(msg->id == MSG_SYN_DONE){												
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_REAGENT_SYRINGE_INIT_POS), REAGENT_INJDRAINCLE_SPEED);  //�س�ʼλ��
							MainState.stateManageBtm = STATE2_CLN_CLN_WAIT;
						}														
					}break;								
		case STATE2_CLN_CLN_WAIT:{
						if(msg->id == MSG_TIMER){			//Ŀǰֻ֧�����ʱ������ڱ�ʱ��			
							GpioSet (BoardGetGpio(IO_VALVE1),GPIO_OFF);	//�ر��ڱڷ�	
							GpioSet (BoardGetGpio(IO_PUMP2),GPIO_OFF); 	//�ر���ڱ�
							StepSYNAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), 0, REAGENT_ZAXISDRAINCLE_RISESPEED);  //z������ԭ��
							StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //����һ���¼�																										
							MainState.stateManageMid = STATE1_CLN_UP;
							MainState.stateManageBtm = STATE2_CLN_CLN_MAX;
						}
					}break;
		default:break;
	}
}
/********************************************************************
 *�Լ����½�-��ϴ-����
 *******************************************************************/
VOID RegeStateClnMessageHandler(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_CLN_BASIC:{
							MainState.Parameter.clnType   = msg->p1;
							MainState.Parameter.innerTime = msg->p2;
							MainState.Parameter.outerTime = msg->p3;
							MainState.stateManageMid = STATE1_CLN_DOWN;									
						}break;
		case STATE1_CLN_DOWN:{																
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_REAGENT_CLEAN_UP_POS), REAGENT_ZAXISDRAINCLE_FALLSPEED); //Z���½�����ϴλ									
							MainState.stateManageMid = STATE1_CLN_CLN;
							MainState.stateManageBtm = STATE2_CLN_CLN_DRA;
						}break;
		case STATE1_CLN_CLN: CleanProcess(msg);	break;		//*ע��* �˴���Ҫ��Ϊ�жϣ��������ȹ��ڱڻ����ȹ����			
		case STATE1_CLN_UP:{
							if(msg->id == MSG_TIR_DONE) {      //������									
								RegeSubAirPluseNum();
								MainState.Parameter.airFlag = 1;
							}
							if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)) {      //��������Һ����ɣ�Z��������									
								MainState.Parameter.airFlag = 0;
								MainState.stateManageMid = STATE1_CLN_MAX;
								RegeStateEndHandler();
							}												
						}break;
		case STATE1_CLN_MAX:break;													
		default:break;
	}
}






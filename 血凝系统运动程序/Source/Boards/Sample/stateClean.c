#include "stateClean.h"

STATIC VOID InnerPumpOn(Message* msg)
{
	if(GpioGet(BoardGetGpio(IO_PUMP1)) == GPIO_OFF)
	{
		if(msg->id == MSG_TIMER2){
			switch(MainState.Parameter.clnType)
			{
				case 1:	TimerStart2(100, ConfigGet(CFG_SAMPLE_INNER_TIME) - 2);break; 	//�ڱڱö�ʱ����	,ʹ�����ñ����ʱ��																	
				case 2:	TimerStart2(100, MainState.Parameter.innerTime - 2);  break;   //�ڱڱö�ʱ���򿪣����趨ʱ��		
				default:break;
			}						
			GpioSet (BoardGetGpio(IO_PUMP1),GPIO_ON);	//���ڱڱ�
		}
	}
	else
	{
		if(msg->id == MSG_TIMER2)
			GpioSet (BoardGetGpio(IO_PUMP1),GPIO_OFF);	//�ر��ڱڱ�
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
									case 1: TimerStart(100,ConfigGet(CFG_SAMPLE_OUTTER_TIME));break; //��ͨ��ϴ //�ڱڷ�����ڱö�ʱ����											 																											
									case 2: TimerStart(100, MainState.Parameter.outerTime);break;	//����ʱ����ϴ //�ڱڷ�����ڱö�ʱ����												 										
									default:break;
								}
								TimerStart2(100, 1);		//��100ms�ڱڱô�								
								StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_SAMPLE_SYRINGE_NEGLMT), SAMPLE_ZAXISDRAINCLE_FALLSPEED);   //����													
								GpioSet (BoardGetGpio(IO_VALVE1),GPIO_ON);		//���ڱڷ�			
								GpioSet (BoardGetGpio(IO_PUMP2),GPIO_ON);		//����ڱ�																																																																	
								MainState.stateManageBtm = STATE2_CLN_CLN_SUB;
							}										
						}break;
		case STATE2_CLN_CLN_SUB:{									
							if(msg->id == MSG_SYN_DONE){												
								StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_SAMPLE_SYRINGE_INIT_POS), SAMPLE_INJDRAINCLE_SPEED);  //�س�ʼλ��
								MainState.stateManageBtm = STATE2_CLN_CLN_WAIT;
							}														
						}break;								
		case STATE2_CLN_CLN_WAIT:{
							if(msg->id == MSG_TIMER){			//Ŀǰֻ֧�����ʱ������ڱ�ʱ��			
								GpioSet (BoardGetGpio(IO_VALVE1),GPIO_OFF);	//�ر��ڱڷ�	
								GpioSet (BoardGetGpio(IO_PUMP2),GPIO_OFF); 	//�ر���ڱ�
								INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
								StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), step0, SAMPLE_ZAXISDRAINCLE_RISESPEED);  //z������ԭ��
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
VOID SampleStateClnMessageHandler(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_CLN_BASIC:break;
		case STATE1_CLN_DOWN:{
								MainState.Parameter.clnType   = msg->p1;
								MainState.Parameter.innerTime = msg->p2;
								MainState.Parameter.outerTime = msg->p3;
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), ConfigGet(CFG_SAMPLE_CLEAN_UP_POS), SAMPLE_ZAXISDRAINCLE_FALLSPEED); //Z���½�����ϴλ									
								MainState.stateManageMid = STATE1_CLN_CLN;
								MainState.stateManageBtm = STATE2_CLN_CLN_DRA;									
							}break;																			
		case STATE1_CLN_CLN: CleanProcess(msg);	break;		//*ע��* �˴���Ҫ��Ϊ�жϣ��������ȹ��ڱڻ����ȹ����			
		case STATE1_CLN_UP:{		
								if(msg->id == MSG_TIR_DONE) {      //������									
									SampleSubAirPluseNum();
									MainState.Parameter.airFlag = 1;
								}
								if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)) {      //��������Һ����ɣ�Z��������									
									MainState.Parameter.airFlag = 0;
									MainState.stateManageMid = STATE1_CLN_MAX;
									SampleStateEndHandler();
								}												
							}break;
		case STATE1_CLN_MAX:break;													
		default:break;
	}
}


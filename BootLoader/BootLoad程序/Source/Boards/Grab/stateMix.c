#include "stateMix.h"
/********************************************************************
 *���û��ȵ����PWM
 *******************************************************************/
STATIC VOID SetPwmDutyRatio(UINT32 ratio)
{
	PwmStop();
	PwmSetDutyRatio (1000 - ratio);
	PwmStart();
}
/*****************************text***********************************/

STATIC VOID MixMotorStrength(UINT16 mix_strength)
{
	INT16 mixpwm = 0;
	if(mix_strength <= 9)//���ȵ�ǿ�ȵȼ�
	{	
		switch(mix_strength)
		{
			case 1:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH1);break;
			case 2:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH2);break;
			case 3:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH3);break;
			case 4:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH4);break;
			case 5:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH5);break;
			case 6:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH6);break;
			case 7:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH7);break;
			case 8:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH8);break;
			case 9:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH9);break;
			case 10:mixpwm = ConfigGet(CFG_GRAB_MIX_STRENGTH10);break;
			default:break;
		}	
		if(OPEN_PWM_ADJUST){		
			TimerStart(MIXUP_TIME_LENGTH, 1);
			SetPwmDutyRatio(MIX_START_PWMVALUE);			//Ϊ��ʼ�Ļ���PWM��21.2%��ռ�ձ�
			MainState.Parameter.mixMaxPwm = mixpwm;    //�������ռ�ձ�	
		}
		else
		{
			TimerStart(100, MainState.Parameter.mixtime/100);
			if(GpioGet(BoardGetGpio(IO_MIX_DRECTION)) == 0)  	  //���ȵ����������Ϊ�͵�ƽ
				SetPwmDutyRatio(mixpwm);
			else if(GpioGet(BoardGetGpio(IO_MIX_DRECTION)) == 1)  //���ȵ����������Ϊ�ߵ�ƽ
				SetPwmDutyRatio(1000 - mixpwm);
		}
	}
}

STATIC VOID MixDoneMessage(VOID)
{
	Message msg;
	msg.id = MSG_MIX_DONE;
	msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
	MessagePost(&msg);
}
	
STATIC VOID MixPwmPlan(Message* msg)
{
	UINT16 upstep = 0,downstep = 0,average_uppwm = 0,max_pwmtime = 0;

	if(OPEN_PWM_ADJUST){
		upstep   = MIXUP_TIME / MIXUP_TIME_LENGTH;
		downstep = MIXDOWN_TIME / MIXDOWN_TIME_LENGTH;
		average_uppwm = (MainState.Parameter.mixMaxPwm - MIX_START_PWMVALUE) / upstep;
		max_pwmtime = MainState.Parameter.mixtime - MIXUP_TIME - MIXDOWN_TIME;
		MainState.Parameter.stepcount++;
		if(MainState.Parameter.stepcount < upstep){
			SetPwmDutyRatio(MIX_START_PWMVALUE + average_uppwm*MainState.Parameter.stepcount);
			TimerStart(MIXUP_TIME_LENGTH, 1);
		}
		else if(MainState.Parameter.stepcount == upstep){
			SetPwmDutyRatio(MainState.Parameter.mixMaxPwm);
			TimerStart(100, max_pwmtime/100);
		}
		else if((MainState.Parameter.stepcount > upstep) && (MainState.Parameter.stepcount <= upstep + downstep)){
			SetPwmDutyRatio(MainState.Parameter.mixMaxPwm - 80.6*(MainState.Parameter.stepcount-upstep));
			TimerStart(MIXDOWN_TIME_LENGTH, 1);
		}
		else if(MainState.Parameter.stepcount >= upstep + downstep + 1){
			MainState.Parameter.stepcount = 0;
			PwmStop();
			MixDoneMessage();
			MainState.stateManageMid = ZAXIS1_DOWN;
		}
	}
	else
	{
		PwmStop();
		MixDoneMessage();
		MainState.stateManageMid = ZAXIS1_DOWN;
	}
}
STATIC VOID MixMotorType(UINT16 type,UINT16 time)
{
	switch(type)
	{
		case 0:MainState.Parameter.mixtime = time*100;break;
		case 1:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_OPTICS_MIX_SAMPLE_TIME)*100;	break;
		case 2:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_OPTICS_MIX_MIX_TIME)*100;		break;
		case 3:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_OPTICS_MIX_TRI_TIME)*100;		break;
		case 4:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_MEGNETIC_MIX_SAMPLE_TIME)*100;	break;
		case 5:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_MEGNETIC_MIX_MID_TIME)*100;		break;
		case 6:MainState.Parameter.mixtime = ConfigGet(CFG_GRAB_MEGNETIC_MIX_TRI_TIME)*100;		break;
		default:break;
	}
}
/********************************************************************
 *���ȵ����ת��������ʱ��
 *******************************************************************/
STATIC VOID MixForward(UINT16 mix_strength,UINT32 mixtype,UINT32 mixtime)
{
	GpioSet (BoardGetGpio(IO_MIX_DRECTION), 0);
	MixMotorType(mixtype,mixtime/100);
	MixMotorStrength(mix_strength);
}
/********************************************************************
 *���ȵ��ֹͣ������ʱ��
 *******************************************************************/
STATIC VOID MixStop(UINT32 mixtime)
{
	GpioSet (BoardGetGpio(IO_MIX_DRECTION), 0);
	TimerStart(100, mixtime/100);
	PwmStop();
}
/********************************************************************
 *���ȵ����ת��������ʱ��
 *******************************************************************/
STATIC VOID MixReversal(UINT16 mix_strength,UINT32 mixtype,UINT32 mixtime)
{
	GpioSet (BoardGetGpio(IO_MIX_DRECTION), 1);
	MixMotorType(mixtype,mixtime/100);
	MixMotorStrength(mix_strength);
}
/********************************************************************
 *���ȵ������
 *******************************************************************/
VOID MixMotorCtrl(Message *msg)
{
	switch(MainState.stateManageMid)
	{
		case MIX1_START:{	
					MainState.Parameter.mixstate = msg->p1;
					MainState.Parameter.mixstrength = msg->p2;
					MainState.Parameter.mixtype = msg->p3;
					MainState.Parameter.mixtime = msg->p4*100/2;    //ת��Ϊ��Ҫ��ms

					switch(MainState.Parameter.mixstate)
					{
						case 0:break; //�ر�
						case 1:break; //��  
						case 2:break; //����
						case 3:{
								MixMotorStrength(MainState.Parameter.mixstrength);
								MainState.stateManageMid = MIX1_PROCESS;
							}break;  //��һ��ʱ��ر�
						case 4:break; //�ر�һ��ʱ���
						case 5:{  //��תһ��ʱ���ת															
								MixForward(MainState.Parameter.mixstrength,MainState.Parameter.mixtype,MainState.Parameter.mixtime);								
								MainState.stateManageMid = CLOSE_MIX;
							}break;							
						case 6:{  //��תһ��ʱ��󣬵ȴ�һ��ʱ�䣬Ȼ��ת
								MixForward(MainState.Parameter.mixstrength,MainState.Parameter.mixtype,MainState.Parameter.mixtime);								
								MainState.stateManageMid = WAIT_MIX;
							}break;
						case 7:{  //��תһ��ʱ��󣬵ȴ�һ��ʱ�䣬Ȼ����ת
								MixReversal(MainState.Parameter.mixstrength,MainState.Parameter.mixtype,MainState.Parameter.mixtime);																
								MainState.stateManageMid = WAIT_MIX;
							}break;
						default:break;
					}													
				}break;
		case WAIT_MIX:{
						if(msg->id == MSG_TIMER){
							MixStop(2000);							
							MainState.stateManageMid = CLOSE_MIX;
						}
					}break;
		case CLOSE_MIX:{
						if(msg->id == MSG_TIMER){
							if((MainState.Parameter.mixstate == 5) || (MainState.Parameter.mixstate == 6)){
								MixReversal(MainState.Parameter.mixstrength,MainState.Parameter.mixtype,MainState.Parameter.mixtime);
							}
							else if(MainState.Parameter.mixstate == 7){
								MixForward(MainState.Parameter.mixstrength,MainState.Parameter.mixtype,MainState.Parameter.mixtime);	
							}
						}
						MainState.stateManageMid = ZAXIS1_MAX;
					}break;
		case MIX1_PROCESS:{
					if(msg->id == MSG_TIMER){
						MixPwmPlan(msg);
					}	
				}break;
		case ZAXIS1_DOWN:{
					if(msg->id == MSG_MIX_DONE){
						MainState.stateManageMid = ZAXIS1_MAX;
						GrabStateEndHandler();
					}
				}break;
		case ZAXIS1_MAX:{
					if(msg->id == MSG_TIMER){
						BoardSetGpio(IO_MIX_DRECTION,GPIO_OFF);
						PwmStop();						
						GrabStateEndHandler();
					}
				}break;
		default:break;
	}
}

/********************************************************************
 *ץ�ֻ���X��
 *******************************************************************/
VOID ZAxisMixMovement(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case ZAXIS1_RISE:{
							MainState.Parameter.mixstrength = msg->p1;
							MainState.Parameter.mixtype = msg->p3;							
							MixMotorType(MainState.Parameter.mixtype,msg->p2);
							StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), ConfigGet(CFG_GRAB_MIX_HIGHT), ZAXIS_MIX_SPEED);
							MainState.stateManageMid = MIX1_START;
						}break;
		case MIX1_START:{
							if(msg->id == MSG_Z_DONE)
							{
								MixMotorStrength(MainState.Parameter.mixstrength);
								MainState.stateManageMid = MIX1_PROCESS;
							}
					    }break;
		case MIX1_PROCESS:{
							if(msg->id == MSG_TIMER){
								MixPwmPlan(msg);
							}
						}break;
		case ZAXIS1_DOWN:{
							if(msg->id == MSG_MIX_DONE){
								StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_GRAB_POS_VRST), ZAXIS_MIX_SPEED);//�ص�Z��̧���߶�
								MainState.stateManageMid = ZAXIS1_MAX;
							}
						}break;
		case ZAXIS1_MAX:{
							if(msg->id == MSG_Z_DONE){
								GrabStateEndHandler();
							}
					}break;
		default:break;
	}
}
/********************************************************************
 *XY�˶�ץ�ֻ���X��
 *******************************************************************/
VOID XYMovementMix(Message *msg)
{
	switch(MainState.stateManageMid)
	{
		case ZAXIS1_BASIC:{
						MainState.Parameter.wrkCode = msg->p1;
						MainState.Parameter.alignhole = msg->p2;
						MainState.stateManageMid = ZAXIS1_RISE;
					}break;
		case ZAXIS1_RISE:{
						MainState.Parameter.mixstrength = msg->p1;						
						MainState.Parameter.mixtype = msg->p3;
						MixMotorType(MainState.Parameter.mixtype,msg->p2);
						GrabWrkStateMessageHandler(1, MainState.Parameter.wrkCode, GRAB_X_YLINKAGEMOTOR1_SPEED,0);    //X���˶�
						GrabWrkStateMessageHandler(2, MainState.Parameter.wrkCode, GRAB_X_YLINKAGEMOTOR2_SPEED,0);				
						StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), ConfigGet(CFG_GRAB_MIX_HIGHT), ZAXIS_MIX_SPEED);  //Z���˶�
						MainState.stateManageMid = MIX1_START;
					}break;
		case MIX1_START:{
						if(msg->id == MSG_Z_DONE)
						{
							MixMotorStrength(MainState.Parameter.mixstrength);
							MainState.stateManageMid = MIX1_PROCESS;
						}
					}break;
		case MIX1_PROCESS:{
						if(msg->id == MSG_TIMER){
							MixPwmPlan(msg);
						}
					}break;
		case ZAXIS1_DOWN:{
							if(msg->id == MSG_MIX_DONE){
								StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_GRAB_POS_VRST), ZAXIS_MIX_SPEED);//�ص�Z��̧���߶�
								MainState.stateManageMid = ZAXIS1_MAX;
							}
					}break;
		case ZAXIS1_MAX:{
							if(msg->id == MSG_Z_DONE){
								MainState.Parameter.mvover = 1;													
							}
					}break;
		default:break;
	}
	if((msg->id == MSG_X_DONE) || (msg->id == MSG_Y_DONE) || (MainState.Parameter.mvover == 1)){
		MainState.Parameter.count1++;
		if(MainState.Parameter.count1 >= 3){
			MainState.Parameter.count1 = 0;
			MainState.Parameter.mvover = 0;
			GrabStateEndHandler();
		}		
	}
}











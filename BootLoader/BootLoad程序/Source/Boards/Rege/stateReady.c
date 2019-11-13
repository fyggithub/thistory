#include "stateReady.h"
/********************************************************************
 *����Һ����
 *******************************************************************/
VOID OpenLiquid(VOID)
{
	GpioSet (BoardGetGpio(IO_LIQUIDE_OUTPUT), 0);  //����Һ����
	LiquidneedleEnable();
}
/********************************************************************
 *�ر�Һ����
 *******************************************************************/
VOID CloseLiquid(VOID)
{      
	GpioSet (BoardGetGpio(IO_LIQUIDE_OUTPUT), 1);  //�ر�Һ����
	LiquidneedleDisable();
}

/********************************************************************
 *
 *******************************************************************/
VOID StepMotorSubAirCallback(VOID)
{
	Message msg;
	INT32 movpos = 0;
	movpos = GetMotorMovPos(&(MainState.motors[MOTOR_Z]));	//����15mm�Ժ󴥷�һ���¼�	
	if(movpos == 736){
		msg.id = MSG_TIR_DONE;
		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
	}		
	if(movpos == -736){   //�½�10mm����
		if(MainState.Parameter.liquidState == 1){
			MainState.Parameter.liquidState = 0;
			OpenLiquid();     //����Һ����		
		}
	}
//	if(GpioGet(BoardGetGpio[IO_LIQUID_COLL]) == 0)         //ײ���ˣ�ֱ���׳�һ���¼�
//	{
//		msg.id = MSG_COLL_DONE;
//		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
//		MessagePost(&msg);
//	}
}
/********************************************************************
 *
 *******************************************************************/
VOID RegeSubAirPluseNum(VOID)
{
	INT32 pluse = 0;
	switch(MainState.stateManageTop)
	{
		case STATE0_SUB:{	
							switch(MainState.Parameter.subType)
							{
								case 1:{
										INT32 subair = ConfigGet(CFG_REAGENT_SUB_SUM_LIQUID) - MainState.Parameter.subVolume*10;
										pluse = (subair * ConfigGet(CFG_REAGENT_STEP_UL)) / 10 ;
									}break;
								case 2:{
										pluse = (ConfigGet(CFG_REAGENT_SUB_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10 ;
									}break;
								default:break;
							}							
							pluse = pluse * (-1);							
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAIN_SPEED);
					   }break;
		case STATE0_DRA:{   								
							pluse = (ConfigGet(CFG_REAGENT_DRAIN_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
							pluse = pluse * (-1);					
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAIN_SPEED);
					   }break;
		case STATE0_CLN:{								
							pluse = (ConfigGet(CFG_REAGENT_CLEAN_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;	
							pluse = pluse * (-1);						
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAINCLE_SPEED);
					   }break;
		default:break;
	}
}
/********************************************************************
 *���е������ͳһ  1ΪԶ����0Ϊ��������     ע������ 1-��  0-��
 *******************************************************************/
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir)
{
	switch(motor)
	{
		case MOTOR_X:{
					if(dir == MOTOR_X_POSITIVE_DIRECTION)   //1ΪԶ������Ϊ������
						step = (-1) * step;
				}break;
		case MOTOR_Y:{
					if(dir == MOTOR_Y_POSITIVE_DIRECTION)   //Y������⣬������Զ����Ϊ�����壬������Ϊ�����壬���������෴
						step = (-1) * step;
				}break;
		case MOTOR_Z:{
					if(dir == MOTOR_Z_POSITIVE_DIRECTION)   //1ΪԶ������Ϊ������
						step = (-1) * step;
				}break;
		case MOTOR_SYN:{
					if(dir == MOTOR_SYN_POSITIVE_DIRECTION)   //1Ϊ��Һ����Ϊ������
						step = (-1) * step;
				}break;
		default:break;
	}
	return step;
}

/********************************************************************
 *��λ��ָ��ִ�����һЩ״̬�������ݷ���
 *******************************************************************/
VOID RegeStateEndHandler(VOID)
{
	SendResultMessage();
}
/********************************************************************
 *Basic״̬,����ָ��������󣬶������˺���
 *******************************************************************/
VOID RegeStateBasicCmdMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_SYN_DONE:{
						RegeStateEndHandler();
					}break;
		default:break;
	}
}
/********************************************************************
 *Z����Һ��
 *******************************************************************/
VOID RegeStateMotorZaixDetMessageHandler(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_LIQUID_DET_INIT:{	
								MainState.Parameter.speed = msg->p2;
								MainState.Parameter.detState = msg->p3;							
								StepZAxisMotorMovRel (&(MainState.motors[MOTOR_Z]), -4800, MainState.Parameter.speed);   //Z����ʻ��Զ����
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);
								MainState.stateManageMid = STATE1_LIQUID_DET_RUN;
								MainState.Parameter.liquidState = 1;   	//����Һ�����IO����
								if(OPEN_SUB_HEAT)
									RegeSendCmdCtlPid(0);  //�����������ر�PID
							}break;
		case STATE1_LIQUID_DET_RUN:{
								switch(msg->id)
								{
									case MSG_DET_DONE:  //Z���⵽Һ��												
													StepMotorBrake(&(MainState.motors[MOTOR_Z]));
													MainState.stateManageMid = STATE1_LIQUID_DET_SUCCESS;
													break;
									case MSG_Z_DONE:	//Z���½���ɣ�δ��⵽Һ��																									     
													CloseLiquid();
													RegeSendCmdCtlPid(1);
													StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, MainState.Parameter.speed);//Z��������ԭ��
													MainState.stateManageMid = STATE1_LIQUID_DET_FALL;
													break;
									case MSG_COLL_DONE: //Z��ײ��
													CloseLiquid();
													RegeSendCmdCtlPid(1);
													StepMotorBrake(&(MainState.motors[MOTOR_Z]));
													MainState.stateManageMid = STATE1_LIQUID_DET_COLL;
													break;
									default:break;													
								}
							}break;
		case STATE1_LIQUID_DET_SUCCESS:{
								if(MainState.Parameter.detState){  				//��⵽Һ���ͣ�ڸ�Һ��																		
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}									
								else{	                    //��⵽Һ��󷵻�ԭ��							
									StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, MainState.Parameter.speed);//Z��������ԭ��
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
								}																												
							}break;
		case STATE1_LIQUID_DET_FALL:{
								if(msg->id == MSG_Z_DONE){ //Z��ص�ԭ�㣬��ʼ�л�״̬																																						
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}
							}break;
		case STATE1_LIQUID_DET_COLL:{
								if(msg->id == MSG_Z_DONE) {//Z��ص�ԭ�㣬��ʼ�л�״̬														
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}
							}break;
		case STATE1_LIQUID_DET_MAX:{
								if((MainState.Parameter.detState == 0) && (msg->id == MSG_Z_DONE)){
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}										
							}break;
		default:break;
	}
}
/********************************************************************
 *Ready״̬�´�����
 *******************************************************************/
VOID RegeStateReadyMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_REGEHOST_MOTOR_ENABLE:{
							UINT8 motor = msg->p1 - 1;			
							StepMotorEnable(&(MainState.motors[motor]));							
							MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_RESET:{	
								MainState.stateManageTop = STATE0_RESET;
								MainState.stateManageMid = STATE1_OPT_INIT;
								RegeStateMotorResetMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_TOZERO:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								switch(motor){
									case 0: StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 1: StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 2: StepZAxisMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 3: StepSYNAxisMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_MOVREL:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								INT32 step = msg->p3;
								INT16 dir = msg->p4;
								step = DirSortOut(motor,step,dir);
								switch(motor){
									case 0:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 1:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 2:StepZAxisMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 3:StepSYNAxisMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;						
						}break;
		case MSG_REGEHOST_MOTOR_MOVABS:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								INT32 step = msg->p3;
								switch(motor){
									case 0:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 1:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 2:StepZAxisMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 3:StepSYNAxisMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_MOVWRK:{
								UINT8 motor = msg->p1;
								UINT16 work_position = msg->p2;
								UINT16 speed = msg->p3;			
								RegeWrkStateMessageHandler(motor, work_position, speed);
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_ZAIXDET:{								
								MainState.stateManageTop = STATE0_ZAIXDET;
								MainState.stateManageMid = STATE1_LIQUID_DET_INIT;						
								RegeStateMotorZaixDetMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_SUBDRA:{
								INT8 dir = msg->p1;
								UINT16 speed = msg->p2;
								INT32 step =  (msg->p3 * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
								step = DirSortOut(MOTOR_SYN,step,dir);
								StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), step, speed);
								MainState.stateManageTop = STATE0_BASIC;
						}break;				
		case MSG_REGEHOST_MOTOR_INQUIRE:break;								
		case MSG_REGEHOST_MOTOR_MOVXY:{		
								MainState.stateManageTop = STATE0_MV;
								MainState.stateManageMid = STATE1_MV_BASIC;
								RegeStateMvMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISSUB:{
								MainState.stateManageTop = STATE0_SUB;
								MainState.stateManageMid = STATE1_SUB_INIT;
								RegeStateSubMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISDRA:{	
								MainState.stateManageTop = STATE0_DRA;
								MainState.stateManageMid = STATE1_DRA_BASIC ;
								RegeStateDraMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISCLE:{	
								MainState.stateManageTop = STATE0_CLN;
								MainState.stateManageMid = STATE1_CLN_BASIC ;			
								RegeStateClnMessageHandler(msg);
						}break;
		default:break;	//something is wrong								
	}
}





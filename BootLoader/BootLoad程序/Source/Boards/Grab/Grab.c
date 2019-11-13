#include "Grab.h"

mainState MainState;
/********************************************************************
 *
 *******************************************************************/
STATIC VOID GrabStateMessageHandler(Message* msg)
{
	switch(MainState.stateManageTop)
	{
		case STATE0_INIT:{	
						if(msg->id == MSG_HOST_MOTOR_RESET){
							MainState.stateManageTop = STATE0_RESET;
							MainState.stateManageMid = STATE1_OPT_INIT;
							GrabStateMotorResetMessageHandler(msg);
						}
					}break;
		case STATE0_RESET:	 GrabStateMotorResetMessageHandler(msg);	break;				
		case STATE0_READY:	 GrabStateReadyMessageHandler(msg);			break;
		case STATE0_BASIC:	 GrabStateBasicCmdMessageHandler(msg);		break;
		case STATE0_MIX_CTRL:MixMotorCtrl(msg);							break;
		case STATE0_MV:		 GrabStateMvMessageHandler(msg);			break;				
		case STATE0_ZAXISPOS:ZAxisSpecifyVerticalPos(msg);		 		break;
		case STATE0_CATCH:	 CatchCup(msg);								break;
		case STATE0_PUT:	 PutCup(msg);								break;
		case STATE0_MIX_TIME:ZAxisMixMovement(msg);						break;
		case STATE0_MV_MIX:	 XYMovementMix(msg);						break;
		case STATE0_THROW:	 ThrowCupMovement(msg);						break;
		case STATE0_DELAY:{
							if(msg->id == MSG_TIMER){
								GrabStateEndHandler();
							}
						}break;
		default:break;
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID GrabMessageHandler(Message* msg)
{
	switch(msg->id)
	{	
		case MSG_HOST_VERSION:	SendResultMessage();		break;
		case MSG_HOST_GETINFO:			break;
		case MSG_HOST_SETINFO:			break;
		case MSG_HOST_PARINFO:{
									MainState.Parameter.codeID1 = msg->p1;
									MainState.Parameter.codeID2 = msg->p2;
									MainState.Parameter.codeID3 = msg->p3;
									MainState.Parameter.codeID4 = msg->p4;
									SendResultMessage();
							}break;
		case MSG_HOST_PARSET:{											
									INT32 id1 = msg->p1;
									INT32 id2 = msg->p2;
									INT32 value1 = msg->p3;
									INT32 value2 = msg->p4;
									if((id1 == 0xFFFF) || (id2 == 0xFFFF)){
										ConfigSetDone();     //CRCУ�鱣��F-RAM
									}
									else{
										if(id1 >= CFG_GRAB_POSLMT_VNEG)
											ConfigSetDword(id1, value1);
										if(id2 >= CFG_GRAB_POSLMT_VNEG)
											ConfigSetDword(id2, value2);
									}
									SendResultMessage();
							}break;
		case MSG_HOST_WRKSAVE:{
									INT16 work = msg->p1;
									INT32 step1 = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
									INT32 step2 = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
									switch(work)
									{
										case GRAB_MEASUREMENTBIT_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MESURE_X_POS1, step1);
																ConfigSetDword(CFG_GRAB_MESURE_Y_POS1, step2);												
															}break;
										case (GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM - 1):{											
																ConfigSetDword(CFG_GRAB_MESURE_X_POS12, step1);
																ConfigSetDword(CFG_GRAB_MESURE_Y_POS12, step2);												
															}break;
										case GRAB_INCUBATIONBIT_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS21, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS21, step2);												
															}break;
										case (GRAB_INCUBATIONBIT_STARTCODE+5):{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS26, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS26, step2);												
															}break;	
										case (GRAB_INCUBATIONBIT_STARTCODE+17):{											
																ConfigSetDword(CFG_GRAB_BORN_X_POS38, step1);
																ConfigSetDword(CFG_GRAB_BORN_Y_POS38, step2);												
															}break;
										case GRAB_MAGNETICBEADCUP_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MEGNETIC_X_POS61, step1);
																ConfigSetDword(CFG_GRAB_MEGNETIC_Y_POS61, step2);												
															}break;
										case (GRAB_MAGNETICBEADCUP_STARTCODE+11):{											
																ConfigSetDword(CFG_GRAB_MEGNETIC_X_POS72, step1);
																ConfigSetDword(CFG_GRAB_MEGNETIC_Y_POS72, step2);												
															}break;
										case GRAB_MAGNETICBEAD_MEASURE_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_MAGMEA_X_POS, step1);
																ConfigSetDword(CFG_GRAB_MEGMEA_Y_POS, step2);												
															}break;
										case GRAB_COOPERATION_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_DISK_X_POS, step1);
																ConfigSetDword(CFG_GRAB_DISK_Y_POS, step2);												
															}break;
										case (GRAB_COOPERATION_STARTCODE+1):{											
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_XPOS, step1);
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_YPOS, step2);												
															}break;
										case (GRAB_COOPERATION_STARTCODE+2):{											
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_XPOS, step1);
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_YPOS, step2);												
															}break;
										case GRAB_THROWCUP_STARTCODE:{											
																ConfigSetDword(CFG_GRAB_THROW_X_POS1, step1);
																ConfigSetDword(CFG_GRAB_THROW_Y_POS1, step2);												
															}break;					
										case (GRAB_THROWCUP_STARTCODE+1):{											
																ConfigSetDword(CFG_GRAB_THROW_X_POS2, step1);
																ConfigSetDword(CFG_GRAB_THROW_Y_POS2, step2);												
															}break;
										case 257:	{	//Z���ʼ�߶�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_POS_VRST, step3);																								
													}break;
										case 258:	{	//Z�����ץ���߶�1
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS, step3);	//��ѧ������ֱλ��																								
													}break;
										case 2818:	{	//Z�����ץ���߶�12
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS12, step3);	//��ѧ������ֱλ��																								
													}break;
										case 5378:	{	//Z�����ץ���߶�21
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS, step3);	//��ѧ������ֱλ��																							
													}break;
										case 6658:	{	//Z�����ץ���߶�26
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS26, step3);	//��ѧ������ֱλ��																							
													}break;
										case 9730:	{	//Z�����ץ���߶�38
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS38, step3);	//��ѧ������ֱλ��																							
													}break;
										case 15618:	{	//���鱭��ֱλ��61
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS, step3);	//���鱭��ֱλ��																						
													}break;
										case 18434:	{	//���鱭��ֱλ��72
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS72, step3);	//���鱭��ֱλ��																						
													}break;
										case 13058:	{	//���鷨����λ��
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGMEA_CATCH_POS, step3);	//���鷨����λ��																						
													}break;
										case 25858:	{	//������
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_DISK_CATCH_POS, step3);	//������																					
													}break;
										case 259:	{	//Z��ű��߶�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MESURE_CATCH_POS, step3);	//��ѧ������ֱλ��																					
													}break;
										case 5379:	{	//Z�����ץ���߶ȣ��ű�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_BORN_CATCH_POS, step3);	//��ѧ������ֱλ��																							
													}break;
										case 10499:	{	//���鱭��ֱλ�ã��ű�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGNETIC_CATCH_POS, step3);	//���鱭��ֱλ��																						
													}break;
										case 13059:	{	//���鷨����λ�ã��ű�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MAGMEA_CATCH_POS, step3);	//���鷨����λ��																						
													}break;
										case 25859:	{	//�����̣��ű�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_DISK_CATCH_POS, step3);	//������																					
													}break;
										case 26116:	{	//Z������Һ�߶ȣ�����
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_COOPERATE_SAMPLE_VERTICAL, step3);																								
													}break;
										case 26372:	{	//Z������Һ�߶ȣ��Լ�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_COOPERATE_REAGENT_VERTICAL, step3);																								
													}break;
										case 261:	{	//Z����ȸ߶�
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_MIX_HIGHT, step3);																								
													}break;
										case 28422:	{	//Z���ױ�λ��
																INT32 step3 = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
																ConfigSetDword(CFG_GRAB_THROW_VERTICAL, step3);																								
													}break;																																																
										default:break;  	 //���͵Ĺ���λ�ô���
									}							
									ConfigSetDone();
									SendResultMessage();
							}break;
		case MSG_HOST_MOTOR_INQUIRE:   SendResultMessage();	 break;
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_MIX_DONE:
		case MSG_HOST_MOTOR_ENABLE:
		case MSG_HOST_MOTOR_RESET:		
		case MSG_HOST_MOTOR_TOZERO:
		case MSG_HOST_MOTOR_MOVREL:
		case MSG_HOST_MOTOR_MOVABS:
		case MSG_HOST_MOTOR_MOVWRK:
		case MSG_HOST_MOTOR_MIXCTL:
		case MSG_HOST_MOTOR_MIXINQUIRE:
		case MSG_HOST_MOTOR_MOVXY:
		case MSG_HOST_MOTOR_ZAXISPOS:
		case MSG_HOST_MOTOR_CATCHCUP:
		case MSG_HOST_MOTOR_PUTCUP:
		case MSG_HOST_MOTOR_MIXTIME:
		case MSG_HOST_MOTOR_MVMIX:
		case MSG_HOST_MOTOR_THROWCUP:	GrabStateMessageHandler(msg);	break;
		case MSG_HOST_MOTOR_PUMPSET:
		case MSG_HOST_MOTOR_PUMPGET:
		case MSG_HOST_MOTOR_VALVESET:
		case MSG_HOST_MOTOR_VALVEGET:
		case MSG_HOST_MOTOR_IOSET:
		case MSG_HOST_MOTOR_IOGET:
		case MSG_HOST_MOTOR_OPTOGET:
		case MSG_HOST_MOTOR_FAULT:		break;
		case MSG_DELAY:{
						MainState.stateManageTop = STATE0_DELAY;	
						INT16 count = msg->p1 / 100;
						TimerStart(100, count);
					}break;
		case MSG_STOP:{
							if(StepMotorGetState(&(MainState.motors[MOTOR_X])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_X]));
							if(StepMotorGetState(&(MainState.motors[MOTOR_Y])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_Y]));
							if(StepMotorGetState(&(MainState.motors[MOTOR_Z])) != MOTOR_STATE_READY)
								StepMotorBrake(&(MainState.motors[MOTOR_Z]));						
																						//ֹͣ���
							memset(&MainState.Parameter,0,sizeof(MainState.Parameter)); //�������															
							PwmStop();													//�رջ��ȵ��
							TimerStop(); 												//�رն�ʱ��
							TimerStop2();
							MessageQueueInit();											//�����Ϣ����
							
							SendResultMessage();										//���ͽ����
							MainState.stateManageTop = STATE0_INIT;    //������ֻ�ܸ�λ,ע���ʱ����λ�������ָ��ȫ�����ش���
						}break;
		default:	break;//something is wrong		
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID GrabInit(VOID)
{
	UINT8 tag = DebugRegister ("grab");
	MessageRegisterHandler (GrabMessageHandler);
	DebugLog(tag, "Grab App Init.");
	
	MainState.motors[MOTOR_X].lpd 	 = BoardGetGpio (IO_LPD1);
	MainState.motors[MOTOR_Y].lpd 	 = BoardGetGpio (IO_LPD2);
	MainState.motors[MOTOR_Z].lpd 	 = BoardGetGpio (IO_LPD3);
	MainState.motors[MOTOR_X].ori 	 = BoardGetGpio (IO_ORI1);
	MainState.motors[MOTOR_Y].ori 	 = BoardGetGpio (IO_ORI2);
	MainState.motors[MOTOR_Z].ori 	 = BoardGetGpio (IO_ORI3);
	MainState.motors[MOTOR_X].opto 	 = BoardGetGpio (IO_OPTO1);
	MainState.motors[MOTOR_Y].opto 	 = BoardGetGpio (IO_OPTO2);
	MainState.motors[MOTOR_Z].opto 	 = BoardGetGpio (IO_OPTO3);
	MainState.motors[MOTOR_X].pulse   =	BoardGetPulse(MOTOR_X);
	MainState.motors[MOTOR_Y].pulse   =	BoardGetPulse(MOTOR_Y);
	MainState.motors[MOTOR_Z].pulse   =	BoardGetPulse(MOTOR_Z);
	MainState.motors[MOTOR_X].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_X);   //��ʱ����Ҫ�ĵ��̨��
	MainState.motors[MOTOR_Y].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Y);
	MainState.motors[MOTOR_Z].speedPlan.accTable   = GetAccelerateTable (ACCTBL_GRAB_Z_DOWN);
	
	StepMotorInit(&(MainState.motors[MOTOR_X]),MSG_X_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Y]),MSG_Y_DONE );
	StepMotorInit(&(MainState.motors[MOTOR_Z]),MSG_Z_DONE );
	
	MainState.stateManageTop = STATE0_READY;
}



#include "stateSub.h"

//吸液-Z轴上升-吸空气
STATIC VOID SampleSubZaxisProcessHandle(Message* msg)
{
	switch(MainState.stateManageEnd)
	{
		case STATE3_UNKNOWN_LIQUID_SUCCESS_INIT:{
								if(msg->id == MSG_Z_DONE){																													
									INT32 sub_pluse = MainState.Parameter.liquidVolume;																										
									StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), sub_pluse, SAMPLE_INJSUBSAM_SPEED);  //吸液
									TimerStart(100, ConfigGet(CFG_SAMPLE_SUB_TIME));
									if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF)
										MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_AIR;
									else if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUB)
										MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_SUB;
								}
							}break;											
		case STATE3_UNKNOWN_LIQUID_SUCCESS_SUB:{
								if(msg->id == MSG_SYN_DONE){
									INT32 pluse = ConfigGet(CFG_SAMPLE_SUB_PUSH);
									StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, SAMPLE_INJSUBSAM_SPEED);  	//回推
									MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_AIR;
								}
						}break;
		case STATE3_UNKNOWN_LIQUID_SUCCESS_AIR:{
							if(msg->id == MSG_TIMER){ //吸液完成							
								INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
								MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step0, SAMPLE_INJSUBSAM_SPEED);
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    		//触发一个事件
								MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_DOWN;
							}
						}break;
		case STATE3_UNKNOWN_LIQUID_SUCCESS_DOWN:{
							if((msg->id == MSG_TIR_DONE) && (MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF)) {      //吸空气									
								SampleSubAirPluseNum();
							}
							if(msg->id == MSG_Z_DONE)
							{
								SampleStateEndHandler();
								MainState.stateManageMid = STATE1_SUB_MAX;
								MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_MAX;
								MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_MAX;												
							}
						}break;
		case STATE3_UNKNOWN_LIQUID_SUCCESS_MAX:break;			
		default:break;
	}
}

//未知液面检测函数
STATIC VOID SampleSubUnknownProcessHandle(Message* msg)
{
	switch(MainState.stateManageBtm)
	{
		case STATE2_UNKNOWN_LIQUID_INIT:{
						switch(msg->id)
						{
							case MSG_DET_DONE:{  //Z轴检测到液面
											StepMotorBrake(&(MainState.motors[MOTOR_Z]));//Z axis stop
											MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_SUCCESS;
											MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_INIT;
										}break;
							case MSG_Z_DONE:{	//Z轴下降完成，未检测到液面
											CloseLiquid();
											MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_FAIL;
											MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
											INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
											StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step0, SAMPLE_INJSUBSAM_SPEED);//Z轴上升回原点
										}break;
							case MSG_COLL_DONE:{ //Z轴撞针
											CloseLiquid();
											MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_COLL;
											StepMotorBrake(&(MainState.motors[MOTOR_Z]));//Z axis stop
										}break;
							default:break;													
						}						
					}break;
		case STATE2_UNKNOWN_LIQUID_SUCCESS:	SampleSubZaxisProcessHandle(msg);	break;																								
		case STATE2_UNKNOWN_LIQUID_FAIL:{
						if(msg->id == MSG_Z_DONE){ //Z轴回到原点，开始切换状态																										
							MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_MAX;
							MainState.stateManageMid = STATE1_SUB_MAX;
							SampleStateEndHandler();
						}							
					}break;
		case STATE2_UNKNOWN_LIQUID_COLL:{
						if(msg->id == MSG_Z_DONE){ //Z轴回到原点，开始切换状态					
							MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_MAX;
							MainState.stateManageMid = STATE1_SUB_MAX;
							MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
							SampleStateEndHandler();
						}
					}break;
		case STATE2_UNKNOWN_LIQUID_MAX:	break;		
		default:break;
	}
}

//已知液面检测函数
STATIC VOID SampleSubKnownProcessHandle(Message* msg)
{
	
}

//下降到绝对高度洗液
STATIC VOID SampleAbsDistanceProcessHandle(Message* msg,INT32 sub_pluse)
{
	switch(MainState.stateManageBtm)
	{
		case STATE2_ABS_DISTANCE_DOWN:{												
								if(msg->id == MSG_Z_DONE){								
									TimerStart(100, ConfigGet(CFG_SAMPLE_SUB_TIME));						
									StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), sub_pluse, SAMPLE_INJSUBSAM_SPEED);
									MainState.stateManageBtm = STATE2_ABS_DISTANCE_SUB;
								}															
							}break;
		case STATE2_ABS_DISTANCE_SUB:{
								if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF){
									if(msg->id == MSG_TIMER){
										INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
										MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
										StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step0, SAMPLE_INJSUBSAM_SPEED);
										StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //吸空气
										MainState.stateManageBtm = STATE2_ABS_DISTANCE_UP;
									}
								}
								else if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUB){
									if(msg->id == MSG_SYN_DONE){
										INT32 pushpluse = ConfigGet(CFG_SAMPLE_SUB_PUSH)*ConfigGet(CFG_SAMPLE_STEP_UL)/10;	//回推
										StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pushpluse, SAMPLE_INJSUBSAM_SPEED);
										MainState.stateManageBtm = STATE2_ABS_DISTANCE_UP;
									}								
								}																						
							}break;
		case STATE2_ABS_DISTANCE_UP:{
							if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF){
								if(msg->id == MSG_TIR_DONE) {      //吸空气									
									SampleSubAirPluseNum();
//									MainState.Parameter.airFlag = 1;
								}
//								if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)){
								if(msg->id == MSG_Z_DONE){
//									MainState.Parameter.airFlag = 0;
									MainState.stateManageBtm = STATE2_ABS_DISTANCE_MAX;
									MainState.stateManageMid = STATE1_SUB_MAX;
									SampleStateEndHandler();
								}
							}
							else if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUB){
								if(msg->id == MSG_SYN_DONE){
									INT32 step1 = ConfigGet(CFG_SAMPLE_POS_VRST);
									StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step1, SAMPLE_INJSUBSAM_SPEED);
									MainState.stateManageBtm = STATE2_ABS_DISTANCE_MAX;																		
								}
							}
						}break;
		case STATE2_ABS_DISTANCE_MAX:{
							if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUB){
								if(msg->id == MSG_Z_DONE){
									MainState.stateManageBtm = STATE2_ABS_DISTANCE_MAX;
									MainState.stateManageMid = STATE1_SUB_MAX;
									SampleStateEndHandler();
								}
							}
						}break;
		default:break;
	}
}
/********************************************************************
 *样本针下降-吸试剂(或吸Buffer)-上升
 *******************************************************************/
VOID SampleStateSubMessageHandler(Message* msg)
{
	INT16 volume = 0; 
	switch(MainState.stateManageMid)
	{
		case STATE1_SUB_INIT:{					
						switch(MainState.Parameter.postcount)
						{
							case 0:{
									if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF)    		//吸-buffer-上升
										volume = msg->p4;						
									else if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISSUB)    //吸样本-上升
										volume = msg->p4 + ConfigGet(CFG_SAMPLE_ADD_SUB);
									MainState.Parameter.liquidVolume = (-1) * volume*ConfigGet(CFG_SAMPLE_STEP_UL) / 10;						
									MainState.Parameter.postcount++;
								}break;
							case 1:{
									MainState.Parameter.postcount = 0;
									MainState.Parameter.downType   = msg->p1;
									MainState.Parameter.mgnVolume  = msg->p2 * (-1);									
									switch(MainState.Parameter.downType)
									{
										case 1:			 //未知液面的液面检测									
												MainState.Parameter.liquidState = 1;
												StepZAxisMotorMovRel (&(MainState.motors[MOTOR_Z]), -9000, SAMPLE_ZAXISSUBSAM_FALLSPEED);   //Z轴行驶最远距离
												StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);
												MainState.stateManageMid = STATE1_UNKNOWN_LIQUID;
												MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_INIT;
												break;    
										case 2: SampleSubKnownProcessHandle(msg);break;    //已知液面的液面检测
										case 3:          //下降到绝对高度位置					
												StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), MainState.Parameter.mgnVolume, SAMPLE_ZAXISSUBSAM_FALLSPEED);
												MainState.stateManageMid = STATE1_ABS_DISTANCE;
												MainState.stateManageBtm = STATE2_ABS_DISTANCE_DOWN;
												break;
										case 4:break;     //随量跟踪液体吸液
										default:break;
									}
								}break;
							default:break;
						}
					}break;													
		case STATE1_UNKNOWN_LIQUID:	SampleSubUnknownProcessHandle(msg);	break;	//未知液面检测						
		case STATE1_KNOWN_LIQUID:	break;						
		case STATE1_ABS_DISTANCE:	SampleAbsDistanceProcessHandle(msg,MainState.Parameter.liquidVolume);	break;	//下降绝对距离						
		case STATE1_FOLLOW_LIQUID:	break;						
		case STATE1_SUB_MAX:		break;						
		default:break;
	}
}




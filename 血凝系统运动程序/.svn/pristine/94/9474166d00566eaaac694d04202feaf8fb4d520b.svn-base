#include "stateSub.h"


//吸液-Z轴上升-吸空气
STATIC VOID RegeSubZaxisProcessHandle(Message* msg)
{
	switch(MainState.stateManageEnd)
	{
		case STATE3_UNKNOWN_LIQUID_SUCCESS_INIT:break;											
		case STATE3_UNKNOWN_LIQUID_SUCCESS_SUB:{
						if(msg->id == MSG_TIMER){							
							INT32 sub_pluse = MainState.Parameter.liquidVolume;
							MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_AIR;																		
							TimerStart(100, ConfigGet(CFG_REAGENT_SUB_TIME));
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), sub_pluse, REAGENT_INJSUBSAM_SPEED);  //吸液
						}
					}break;
		case STATE3_UNKNOWN_LIQUID_SUCCESS_AIR:{
						if(msg->id == MSG_TIMER){  //吸液时间到			
							MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
							StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, REAGENT_INJSUBSAM_SPEED);
							StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //触发一个事件
							MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_DOWN;
						}
					}break;
		case STATE3_UNKNOWN_LIQUID_SUCCESS_DOWN:{
						if(msg->id == MSG_TIR_DONE) {      //吸空气									
							RegeSubAirPluseNum();
//							MainState.Parameter.airFlag = 1;
						}
//						if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)){
						if(msg->id == MSG_Z_DONE){
//							MainState.Parameter.airFlag = 0;
							RegeStateEndHandler();
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
STATIC VOID RegeSubUnknownProcessHandle(Message* msg)
{
	switch(MainState.stateManageBtm)
	{
		case STATE2_UNKNOWN_LIQUID_INIT:{
					switch(msg->id)
					{
						case MSG_DET_DONE:  //Z轴检测到液面
										if(OPEN_SUB_HEAT){
											RegeSendSlaveCmdMode(GetSubPwm(),REGE_HEAT_TIME,REGE_STOP_TIME);   //检测到液面，告诉液面检测开启开环加热
										}
										StepMotorBrake(&(MainState.motors[MOTOR_Z]));
										MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_SUCCESS;
										MainState.stateManageEnd = STATE3_UNKNOWN_LIQUID_SUCCESS_SUB;
										break;
						case MSG_Z_DONE:	//Z轴下降完成，未检测到液面
										CloseLiquid();
										MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_FAIL;
										MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
										StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, REAGENT_INJSUBSAM_SPEED);//Z轴上升回原点
										break;
						case MSG_COLL_DONE: //Z轴撞针
										CloseLiquid();
										MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_COLL;
										StepMotorBrake(&(MainState.motors[MOTOR_Z]));//Z axis stop
										break;
						default:break;													
					}						
				}break;
		case STATE2_UNKNOWN_LIQUID_SUCCESS:	RegeSubZaxisProcessHandle(msg);	break;																					
		case STATE2_UNKNOWN_LIQUID_FAIL:{
					if(msg->id == MSG_Z_DONE) //Z轴回到原点，开始切换状态
					{																						
						MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_MAX;
						MainState.stateManageMid = STATE1_SUB_MAX;
						RegeStateEndHandler();
					}							
				}break;
		case STATE2_UNKNOWN_LIQUID_COLL:{
					if(msg->id == MSG_Z_DONE) //Z轴回到原点，开始切换状态
					{
						MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_MAX;
						MainState.stateManageMid = STATE1_SUB_MAX;
						MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
						RegeStateEndHandler();
					}
				}break;
		case STATE2_UNKNOWN_LIQUID_MAX:	break;		
		default:break;
	}
}
//已知液面检测函数
STATIC VOID RegeSubKnownProcessHandle(Message* msg)
{
	
}

//下降到绝对高度洗液
STATIC VOID RegeAbsDistanceProcessHandle(Message* msg,INT32 sub_pluse)
{
	switch(MainState.stateManageBtm)
	{
		case STATE2_ABS_DISTANCE_DOWN:{
							if(msg->id == MSG_TIMER){
								TimerStart(100,ConfigGet(CFG_REAGENT_SUB_TIME));						
								StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), sub_pluse, REAGENT_INJSUBSAM_SPEED);
								MainState.stateManageBtm = STATE2_ABS_DISTANCE_SUB;
							}											
						}break;
		case STATE2_ABS_DISTANCE_SUB:{		
							if(msg->id == MSG_TIMER){
								MainState.Parameter.zLowestPulse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, REAGENT_INJSUBSAM_SPEED);
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //触发一个事件
								MainState.stateManageBtm = STATE2_ABS_DISTANCE_UP;
							}										
						}break;
		case STATE2_ABS_DISTANCE_UP:{
							if(msg->id == MSG_TIR_DONE){      //吸空气									
								RegeSubAirPluseNum();
//								MainState.Parameter.airFlag = 1;
							}									
//							if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)){
							if(msg->id == MSG_Z_DONE){
//								MainState.Parameter.airFlag = 0;
								MainState.stateManageBtm = STATE2_ABS_DISTANCE_MAX;
								MainState.stateManageMid = STATE1_SUB_MAX;
								RegeStateEndHandler();
							}
						}break;
		case STATE2_ABS_DISTANCE_MAX:break;
		default:break;
	}
}

/********************************************************************
 *试剂针下降-吸试剂-上升
 *******************************************************************/
VOID RegeStateSubMessageHandler(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_SUB_INIT:{
								MainState.Parameter.subType = msg->p3;
								MainState.Parameter.liquidVolume = (-1)*(msg->p4*ConfigGet(CFG_REAGENT_STEP_UL))/10;																
								MainState.Parameter.subVolume = (FLOAT)msg->p4 / 10;
								MainState.stateManageMid = STATE1_SUB_REV;
							}break;
		case STATE1_SUB_REV:{
								MainState.Parameter.downType   = msg->p1;
								MainState.Parameter.mgnVolume  = msg->p2 * (-1);
								TimerStart(100, ConfigGet(CFG_REAGENT_SUB_DOWN_TIME));  //打开定时器，固定时间下降
								switch(MainState.Parameter.downType)
								{
									case 1:{			 //未知液面的液面检测									
											MainState.Parameter.liquidState = 1;   	//开启液面检测的IO引脚
											StepZAxisMotorMovRel (&(MainState.motors[MOTOR_Z]), -4800, REAGENT_ZAXISSUBSAM_FALLSPEED);   //Z轴行驶最远距离
											StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);
											MainState.stateManageMid = STATE1_UNKNOWN_LIQUID;
											MainState.stateManageBtm = STATE2_UNKNOWN_LIQUID_INIT;																					
											if(OPEN_SUB_HEAT)
												RegeSendCmdCtlPid(0);  //开启开环，关闭PID
										}break;    
									case 2: RegeSubKnownProcessHandle(msg);break;    //已知液面的液面检测
									case 3: break;         //随量跟踪液体吸液																																								
									case 4:{
												StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), MainState.Parameter.mgnVolume, REAGENT_ZAXISSUBSAM_FALLSPEED);
												MainState.stateManageMid = STATE1_ABS_DISTANCE;
												MainState.stateManageBtm = STATE2_ABS_DISTANCE_DOWN;
										}break;     //下降到绝对高度位置		
									default:break;
								}
							}break;
		case STATE1_UNKNOWN_LIQUID:	RegeSubUnknownProcessHandle(msg);	break;	//未知液面检测						
		case STATE1_KNOWN_LIQUID:	break;						
		case STATE1_ABS_DISTANCE:	RegeAbsDistanceProcessHandle(msg,MainState.Parameter.liquidVolume);	break;	//下降绝对距离						
		case STATE1_FOLLOW_LIQUID:	break;						
		case STATE1_SUB_MAX:		break;						
		default:break;
	}
}


#include "stateDrain.h"

/********************************************************************
 *试剂针下降-排试剂-上升
 *******************************************************************/
VOID RegeStateDraMessageHandler(Message* msg)
{
	INT32 drain_liquidvolume = 0;	
	switch(MainState.stateManageMid)
	{
		case STATE1_DRA_BASIC:{
							MainState.Parameter.draVolume = msg->p1;
//							drain_liquidvolume = MainState.Parameter.draVolume + ConfigGet(CFG_REAGENT_SUB_LATER_AIR) + 25 ;    //最终排液量 = 排液量+吸空气量+2.5
							drain_liquidvolume = MainState.Parameter.draVolume + 25 ; //最终排液量 = 排液量+2.5
							MainState.Parameter.drainPluse = (drain_liquidvolume * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
							MainState.Parameter.dirType = msg->p3;
							switch(MainState.Parameter.dirType)
							{
								case 0:{
										INT32 step = ConfigGet(CFG_REAGENT_COOPERATE_UP_POS) + 738;    //距离15mmZ轴停止，开始第一次排液
										StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]),step, REAGENT_ZAXISDRAIN_FALLSPEED);
										MainState.stateManageMid = STATE1_DRA_DRA;
									}break;
								case 1:{															
										StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_REAGENT_COOPERATE_UP_POS), REAGENT_ZAXISDRAIN_FALLSPEED);
										MainState.stateManageMid = STATE1_DRA_DOWN1;
									}break;
								default:break;
							}							
						}break;
		case STATE1_DRA_DRA:{
								if(msg->id == MSG_Z_DONE){
									INT32 pluse = ConfigGet(CFG_REAGENT_SUB_SUM_LIQUID) - MainState.Parameter.draVolume;
									pluse = (pluse * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
									StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAIN_SPEED);     //注意排液的方向
									MainState.stateManageMid = STATE1_DRA_DOWN2;
								}
						}break;
		case STATE1_DRA_DOWN2:{
								if(msg->id == MSG_SYN_DONE){									
									StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_REAGENT_COOPERATE_UP_POS), REAGENT_ZAXISDRAIN_FALLSPEED);
									MainState.stateManageMid = STATE1_DRA_DOWN1;
								}
						}break;
		case STATE1_DRA_DOWN1:{	
							switch(MainState.Parameter.dirType){
								case 0:{
											if(msg->id == MSG_Z_DONE){
												TimerStart2(40, 1);   //延时40ms												
												MainState.stateManageMid = STATE1_DRA_WAIT1;
											}	
									}break;
								case 1:{
											if(msg->id == MSG_Z_DONE){								
												TimerStart(100, ConfigGet(CFG_REAGENT_DRAIN_TIME));
												TimerStart2(40, 1);   //延时40ms												
												MainState.stateManageMid = STATE1_DRA_WAIT1;
											}
									}break;
								default:break;
							}							
						}break;
		case STATE1_DRA_WAIT1:{	
							if(msg->id == MSG_TIMER2){				
								StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), MainState.Parameter.drainPluse, REAGENT_INJDRAIN_SPEED);     //注意排液的方向
								switch(MainState.Parameter.dirType){
									case 0:MainState.stateManageMid = STATE1_DRA_WAIT2; break;  //延时40ms
									case 1:MainState.stateManageMid = STATE1_DRA_UP; 	break;
									default:break;
								}								
							}						
						}break;
		case STATE1_DRA_WAIT2:{
							if(msg->id == MSG_SYN_DONE){
								TimerStart(40, 1); 								
								MainState.stateManageMid = STATE1_DRA_UP;
							}
						}break;
		case STATE1_DRA_UP:{	
							if(msg->id == MSG_TIMER){									
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, REAGENT_ZAXISDRAIN_FALLSPEED);  //motor dra1
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //触发一个事件
								MainState.stateManageMid = STATE1_DRA_MAX;
							}
						}break;
		case STATE1_DRA_MAX:{	
							if(msg->id == MSG_TIR_DONE) {      //吸空气									
								RegeSubAirPluseNum();
								MainState.Parameter.airFlag = 1;
							}
							if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)){           //此处必须保证Z轴最后完成
								MainState.Parameter.airFlag = 0;
								MainState.stateManageMid = STATE1_DRA_MAX;
								RegeStateEndHandler();
							}
						}break;
		default:break;
	}
}



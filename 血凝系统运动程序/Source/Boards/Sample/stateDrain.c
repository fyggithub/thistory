#include "stateDrain.h"

//随量跟踪液体吸液
VOID SampleFollowProcessHandle(Message* msg)
{
	
}

STATIC INT16 ZAxis_Fall_Pulses(VOID)
{
	INT32 pluse1 = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	INT32 pluse2 = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	if( (pluse1 == ConfigGet(CFG_SAMPLE_DISK_X_POS)) && \
	    (pluse2 == ConfigGet(CFG_SAMPLE_DISK_Y_POS)) )   //判断当前的工作位置是否在参数202的位置
	{
		return ConfigGet(CFG_SAMPLE_DISK_UP_POS);
	}
	else if( (pluse1 == ConfigGet(CFG_SAMPLE_COOPERATE_X_POS)) && \
		     (pluse2 == ConfigGet(CFG_SAMPLE_COOPERATE_Y_POS)) )   //判断当前工作位置是否在参数203的位置
	{ 
		return ConfigGet(CFG_SAMPLE_COOPERATE_UP_POS);
	}
	return 0;
}

/********************************************************************
 *试剂针下降-排试剂-上升
 *******************************************************************/
VOID SampleStateDraMessageHandler(Message* msg)
{
	INT32 drain_liquidvolume = 0;	
	switch(MainState.stateManageMid)
	{
		case STATE1_DRA_BASIC:{
								if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISDRA)
									drain_liquidvolume = msg->p1; 
								else if(MainState.Parameter.receiveCmd == SAMPLEHOST_CMD_MOTOR_ZAXISDRABUF)
									drain_liquidvolume = msg->p1 + msg->p2;
								MainState.Parameter.drianPulse = (drain_liquidvolume * ConfigGet(CFG_SAMPLE_STEP_UL)) / 10;
								INT32 upstep = ZAxis_Fall_Pulses();
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), upstep, SAMPLE_ZAXISDRAIN_FALLSPEED);
								MainState.stateManageMid = STATE1_DRA_DOWN;
							}break;
		case STATE1_DRA_DOWN:	
							if(msg->id == MSG_Z_DONE){								
								TimerStart(100, ConfigGet(CFG_SAMPLE_DRAIN_TIME));
								TimerStart2(40, 1);   //延时40ms
								MainState.stateManageMid = STATE1_DRA_WAIT;
							}							
							break;
		case STATE1_DRA_WAIT:	
							if(msg->id == MSG_TIMER2){				
								StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), MainState.Parameter.drianPulse, SAMPLE_INJDRAIN_SPEED);     //注意排液的方向
								MainState.stateManageMid = STATE1_DRA_UP;
							}						
							break;
		case STATE1_DRA_UP:	
							if(msg->id == MSG_TIMER){							
								INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step0, 16);  //motor dra1
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //触发一个事件
								MainState.stateManageMid = STATE1_DRA_MAX;
							}
							break;
		case STATE1_DRA_MAX:{
							if(msg->id == MSG_TIR_DONE) {      //吸空气									
								SampleSubAirPluseNum();
								MainState.Parameter.airFlag = 1;
							}
							if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)){           //此处必须保证Z轴最后完成
								MainState.Parameter.airFlag = 0;
								MainState.stateManageMid = STATE1_DRA_MAX;
								SampleStateEndHandler();
							}
						}break;
		default:break;
	}
}








#include "stateClean.h"

STATIC VOID InnerPumpOn(Message* msg)
{
	if(GpioGet(BoardGetGpio(IO_PUMP1)) == GPIO_OFF)
	{
		if(msg->id == MSG_TIMER2){
			switch(MainState.Parameter.clnType)
			{
				case 1:	TimerStart2(100, ConfigGet(CFG_SAMPLE_INNER_TIME) - 2);break; 	//内壁泵定时器打开	,使用配置表里的时间																	
				case 2:	TimerStart2(100, MainState.Parameter.innerTime - 2);  break;   //内壁泵定时器打开，自设定时间		
				default:break;
			}						
			GpioSet (BoardGetGpio(IO_PUMP1),GPIO_ON);	//打开内壁泵
		}
	}
	else
	{
		if(msg->id == MSG_TIMER2)
			GpioSet (BoardGetGpio(IO_PUMP1),GPIO_OFF);	//关闭内壁泵
	}
}

STATIC VOID CleanProcess(Message* msg)
{
	InnerPumpOn(msg);   //内壁泵的时间控制
	switch(MainState.stateManageBtm)
	{
		case STATE2_CLN_CLN_DRA:{
							if(msg->id == MSG_Z_DONE){
								switch(MainState.Parameter.clnType)
								{
									case 1: TimerStart(100,ConfigGet(CFG_SAMPLE_OUTTER_TIME));break; //普通清洗 //内壁阀、外壁泵定时器打开											 																											
									case 2: TimerStart(100, MainState.Parameter.outerTime);break;	//自设时间清洗 //内壁阀、外壁泵定时器打开												 										
									default:break;
								}
								TimerStart2(100, 1);		//等100ms内壁泵打开								
								StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_SAMPLE_SYRINGE_NEGLMT), SAMPLE_ZAXISDRAINCLE_FALLSPEED);   //回推													
								GpioSet (BoardGetGpio(IO_VALVE1),GPIO_ON);		//打开内壁阀			
								GpioSet (BoardGetGpio(IO_PUMP2),GPIO_ON);		//打开外壁泵																																																																	
								MainState.stateManageBtm = STATE2_CLN_CLN_SUB;
							}										
						}break;
		case STATE2_CLN_CLN_SUB:{									
							if(msg->id == MSG_SYN_DONE){												
								StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_SAMPLE_SYRINGE_INIT_POS), SAMPLE_INJDRAINCLE_SPEED);  //回初始位置
								MainState.stateManageBtm = STATE2_CLN_CLN_WAIT;
							}														
						}break;								
		case STATE2_CLN_CLN_WAIT:{
							if(msg->id == MSG_TIMER){			//目前只支持外壁时间大于内壁时间			
								GpioSet (BoardGetGpio(IO_VALVE1),GPIO_OFF);	//关闭内壁阀	
								GpioSet (BoardGetGpio(IO_PUMP2),GPIO_OFF); 	//关闭外壁泵
								INT32 step0 = ConfigGet(CFG_SAMPLE_POS_VRST);
								StepZAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), step0, SAMPLE_ZAXISDRAINCLE_RISESPEED);  //z轴电机回原点
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);    //触发一个事件																										
								MainState.stateManageMid = STATE1_CLN_UP;
								MainState.stateManageBtm = STATE2_CLN_CLN_MAX;
							}
						}break;
		default:break;
	}
}

/********************************************************************
 *试剂针下降-清洗-上升
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
								StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), ConfigGet(CFG_SAMPLE_CLEAN_UP_POS), SAMPLE_ZAXISDRAINCLE_FALLSPEED); //Z轴下降到清洗位									
								MainState.stateManageMid = STATE1_CLN_CLN;
								MainState.stateManageBtm = STATE2_CLN_CLN_DRA;									
							}break;																			
		case STATE1_CLN_CLN: CleanProcess(msg);	break;		//*注意* 此处需要人为判断，程序是先关内壁还是先关外壁			
		case STATE1_CLN_UP:{		
								if(msg->id == MSG_TIR_DONE) {      //吸空气									
									SampleSubAirPluseNum();
									MainState.Parameter.airFlag = 1;
								}
								if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)) {      //必须是吸液先完成，Z轴最后完成									
									MainState.Parameter.airFlag = 0;
									MainState.stateManageMid = STATE1_CLN_MAX;
									SampleStateEndHandler();
								}												
							}break;
		case STATE1_CLN_MAX:break;													
		default:break;
	}
}


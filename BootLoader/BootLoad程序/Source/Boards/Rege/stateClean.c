#include "stateClean.h"

/********************************************************************
 *计算清洗时预热PWM的公式
 *******************************************************************/
INT16 GetClnPwm(VOID)
{
	INT16 temp;
	temp = sqrt(0.03687 * K1 * (37 - Ta) / CLN_HEAT_TIME * 1000) * 1000;
	return temp;
}
/********************************************************************
 *计算试剂时预热PWM的公式
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
					case 1:TimerStart2(100, time);break;		//内壁泵定时器打开						
					case 2:TimerStart2(100, time);break;			//内壁泵定时器打开		
					default:break;
				}										
				GpioSet (BoardGetGpio(IO_PUMP1),GPIO_ON);				 //打开内壁泵
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
						}break;		//内壁泵定时器打开						
					case 2:{
							time = MainState.Parameter.innerTime - 2 - OPEN_HEAT_TIME / 100;
							TimerStart2(100, time);				
						}break;			//内壁泵定时器打开		
					default:break;
				}
				if(OPEN_CLN_HEAT)
					RegeSendSlaveCmdMode(GetClnPwm(),CLN_HEAT_TIME,CLN_STOP_TIME);   //发送指令告诉液面检测可以开始预热		
			}
			else
			{
				MainState.Parameter.countpump = 0;
				GpioSet (BoardGetGpio(IO_PUMP1),GPIO_OFF);	//关闭内壁泵
			}
		}			
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
								case 1:TimerStart(100, ConfigGet(CFG_REAGENT_OUTTER_TIME));break; //普通清洗   //内壁阀、外壁泵定时器打开//主要是定时器一次最多能定时1200ms																			  									
								case 2:TimerStart(100, MainState.Parameter.outerTime);break; 		//自设时间清洗						
								default:break;
							}
							TimerStart2(100, 1);		//等100ms内壁泵打开								
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_REAGENT_SYRINGE_NEGLMT), REAGENT_ZAXISDRAINCLE_FALLSPEED);   //回推													
							GpioSet (BoardGetGpio(IO_VALVE1),GPIO_ON);		//打开内壁阀			
							GpioSet (BoardGetGpio(IO_PUMP2),GPIO_ON);		//打开外壁泵																																																																	
							MainState.stateManageBtm = STATE2_CLN_CLN_SUB;
						}										
					}break;
		case STATE2_CLN_CLN_SUB:{									
						if(msg->id == MSG_SYN_DONE){												
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_SYN]), ConfigGet(CFG_REAGENT_SYRINGE_INIT_POS), REAGENT_INJDRAINCLE_SPEED);  //回初始位置
							MainState.stateManageBtm = STATE2_CLN_CLN_WAIT;
						}														
					}break;								
		case STATE2_CLN_CLN_WAIT:{
						if(msg->id == MSG_TIMER){			//目前只支持外壁时间大于内壁时间			
							GpioSet (BoardGetGpio(IO_VALVE1),GPIO_OFF);	//关闭内壁阀	
							GpioSet (BoardGetGpio(IO_PUMP2),GPIO_OFF); 	//关闭外壁泵
							StepSYNAxisMotorMovAbs(&(MainState.motors[MOTOR_Z]), 0, REAGENT_ZAXISDRAINCLE_RISESPEED);  //z轴电机回原点
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
							StepSYNAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]),ConfigGet(CFG_REAGENT_CLEAN_UP_POS), REAGENT_ZAXISDRAINCLE_FALLSPEED); //Z轴下降到清洗位									
							MainState.stateManageMid = STATE1_CLN_CLN;
							MainState.stateManageBtm = STATE2_CLN_CLN_DRA;
						}break;
		case STATE1_CLN_CLN: CleanProcess(msg);	break;		//*注意* 此处需要人为判断，程序是先关内壁还是先关外壁			
		case STATE1_CLN_UP:{
							if(msg->id == MSG_TIR_DONE) {      //吸空气									
								RegeSubAirPluseNum();
								MainState.Parameter.airFlag = 1;
							}
							if((msg->id == MSG_Z_DONE) && (MainState.Parameter.airFlag == 1)) {      //必须是吸液先完成，Z轴最后完成									
								MainState.Parameter.airFlag = 0;
								MainState.stateManageMid = STATE1_CLN_MAX;
								RegeStateEndHandler();
							}												
						}break;
		case STATE1_CLN_MAX:break;													
		default:break;
	}
}






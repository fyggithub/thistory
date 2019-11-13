#include "stateMoveWork.h"

////需要机械提供图纸坐标 2#机械图纸
//CONST FLOAT AxisPos[22][2] = {   			//图纸上各点坐标
//	{80.6,2.8},            //大孔区1号位置
//	{80.6,43.7},           //大孔区2号位置
//	{35,0},           	   //中孔区1号位置
//	{0,0},                 //中孔区2号位置
//	{35,34},               //中孔区3号位置
//	{0,34},                //中孔区4号位置
//	{35,68},               //中孔区5号位置
//	{0,68},                //中孔区6号位置
//	{35,102},              //中孔区7号位置
//	{0,102},               //中孔区8号位置
//	{35,136},              //中孔区9号位置
//	{0,136},               //中孔区10号位置
//	{93.6,80},             //小孔区1号位置
//	{67.6,80},             //小孔区2号位置
//	{93.6,106.5},          //小孔区3号位置
//	{67.6,106.5},          //小孔区4号位置
//	{93.6,133},            //小孔区5号位置
//	{67.6,133},            //小孔区6号位置
//	{93.6,159.5},          //小孔区7号位置
//	{67.6,159.5},          //小孔区8号位置
//	{41.5,164.5},          //小孔区9号位置
//	{15.5,164.5}           //小孔区10号位置
//};

//需要机械提供图纸坐标 3#机械图纸
CONST FLOAT AxisPos[28][2] = {   			//图纸上各点坐标
	{115.3,0},            	//大孔区1号位置
	{115.3,46.5},           //大孔区2号位置
	{70,0},           	   	//中孔区1号位置
	{35,0},                 //中孔区2号位置
	{0,0},               	//中孔区3号位置
	{70,35},                //中孔区4号位置
	{35,35},               	//中孔区5号位置
	{0,35},                	//中孔区6号位置
	{70,70},              	//中孔区7号位置
	{35,70},               	//中孔区8号位置
	{0,70},              	//中孔区9号位置
	{70,105},               //中孔区10号位置
	{35,105},               //中孔区11号位置
	{0,105},               	//中孔区12号位置
	{129.3,80.5},           //小孔区1号位置
	{101.3,80.5},           //小孔区2号位置
	{129.3,108.5},          //小孔区3号位置
	{101.3,108.5},          //小孔区4号位置
	{129.3,136.5},          //小孔区5号位置
	{101.3,136.5},          //小孔区6号位置
	{129.3,164.5},          //小孔区7号位置
	{101.3,164.5},          //小孔区8号位置
	{70,136.5},          	//小孔区9号位置
	{35,136.5},           	//小孔区10号位置
	{0,136.5},           	//小孔区11号位置
	{70,164.5},           	//小孔区12号位置
	{35,164.5},           	//小孔区13号位置
	{0,164.5}           	//小孔区14号位置
};

////需要机械提供图纸坐标 1#机械图纸
//CONST FLOAT AxisPos[22][2] = {   			//图纸上各点坐标
//	{0,0},            //大孔区1号位置
//	{0,0},           //大孔区2号位置
//	{0,136},           	   //中孔区1号位置
//	{0,0},                 //中孔区2号位置
//	{0,136},               //中孔区3号位置
//	{0,136},                //中孔区4号位置
//	{0,136},               //中孔区5号位置
//	{0,136},                //中孔区6号位置
//	{0,136},              //中孔区7号位置
//	{0,136},               //中孔区8号位置
//	{0,136},              //中孔区9号位置
//	{0,136},               //中孔区10号位置
//	{93.6,80},             //小孔区1号位置
//	{93.6,80},             //小孔区2号位置
//	{93.6,80},          	//小孔区3号位置
//	{93.6,80},          	//小孔区4号位置
//	{93.6,80},            //小孔区5号位置
//	{93.6,80},            //小孔区6号位置
//	{93.6,80},          //小孔区7号位置
//	{93.6,80},          //小孔区8号位置
//	{93.6,80},          //小孔区9号位置
//	{93.6,80},           //小孔区10号位置
//};

STATIC INT32 GetWorkPos(INT32 wrkPos,INT32 pos_num)
{
	FLOAT M[3][3],axis_y1,axis_x2,axis_y2;
	INT32 pos_x[3],pos_y[3];
	INT32 pos[2];
	INT16 num;
	
	if(wrkPos == REAGENT_LEVEL_CODE){
		pos[0] = ConfigGet(CFG_REAGENT_CLEAN_X_POS);
		pos[1] = ConfigGet(CFG_REAGENT_CLEAN_Y_POS);
	}
	else if(wrkPos == REAGENT_LEVEL_CODE+1){
		pos[0] = ConfigGet(CFG_REAGENT_COOPERATE_X_POS);
		pos[1] = ConfigGet(CFG_REAGENT_COOPERATE_Y_POS);
	}
	else{	
		if((wrkPos >= REAGENT_BIGHOLE_CODE) && \
		   (wrkPos < REAGENT_BIGHOLE_CODE + REAGENT_BIGHOLE_NUM)){
			num = wrkPos - REAGENT_BIGHOLE_CODE;	
		}
		else if((wrkPos >= REAGENT_MIDHOLE_CODE) && \
				(wrkPos < REAGENT_MIDHOLE_CODE + REAGENT_MIDHOLE_NUM)){
			num = (wrkPos - REAGENT_MIDHOLE_CODE) + 2;		
		}
		else if((wrkPos >= REAGENT_SMALLHOLE_CODE) && \
				(wrkPos < REAGENT_SMALLHOLE_CODE + REAGENT_SMALLHOLE_NUM)){
			num = (wrkPos - REAGENT_SMALLHOLE_CODE) + 14;		
		}	 
		else{
			pos[0] = pos[1] = 0;
			return (pos[pos_num]);
		}	
		pos_x[0] = ConfigGet(CFG_REAGENT_COOL_X_POS1);
		pos_x[1] = ConfigGet(CFG_REAGENT_COOL_X_POS2);
		pos_x[2] = ConfigGet(CFG_REAGENT_COOL_X_POS3);
		pos_y[0] = ConfigGet(CFG_REAGENT_COOL_Y_POS1);
		pos_y[1] = ConfigGet(CFG_REAGENT_COOL_Y_POS2);
		pos_y[2] = ConfigGet(CFG_REAGENT_COOL_Y_POS3);	
		axis_y1 = AxisPos[13][1];    //图纸上y1坐标  ，中孔12号位置
		axis_x2 = AxisPos[14][0];    //图纸上x2坐标  ，小孔1号位置
		axis_y2 = AxisPos[14][1];    //图纸上y2坐标  ，小孔1号位置
		
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = (FLOAT)(pos_x[2] - M[2][0] - axis_y2*M[1][0]) / axis_x2;
		M[0][1] = (FLOAT)(pos_y[2] - M[2][1] - axis_y2*M[1][1]) / axis_x2;
		
		pos[0] = (INT32)(AxisPos[num][0]*M[0][0] + AxisPos[num][1]*M[1][0] + M[2][0]);
		pos[1] = (INT32)(AxisPos[num][0]*M[0][1] + AxisPos[num][1]*M[1][1] + M[2][1]);
	}
	return pos[pos_num];
}
/********************************************************************
 *
 *******************************************************************/
VOID RegeWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed)
{
	INT32 step = 0;
	switch(motor - 1)
	{
		case MOTOR_X:{								
						step = GetWorkPos(work_position,0);
						StepMotorMovAbs (&(MainState.motors[MOTOR_X]), step, speed);							
					}break;											
		case MOTOR_Y:{							
						step = GetWorkPos(work_position,1);			
						StepMotorMovAbs (&(MainState.motors[MOTOR_Y]), step, speed);					
					}break;
		case MOTOR_Z:{
						if((work_position >= REAGENT_BIGHOLE_CODE) && \
						   (work_position < REAGENT_BIGHOLE_CODE + REAGENT_BIGHOLE_NUM)){
							step = ConfigGet(CFG_REAGENT_BIG_HOLE_UP_POS);	
						}
						else if((work_position >= REAGENT_MIDHOLE_CODE) && \
								(work_position < REAGENT_MIDHOLE_CODE + REAGENT_MIDHOLE_NUM)){
							step = ConfigGet(CFG_REAGENT_MID_HOLE_UP_POS);	
						}
						else if((work_position >= REAGENT_SMALLHOLE_CODE) && \
								(work_position < REAGENT_SMALLHOLE_CODE + REAGENT_SMALLHOLE_NUM)){
							step = ConfigGet(CFG_REAGENT_SMALL_HOLE_UP_POS);		
						}	 
						else if(work_position == REAGENT_LEVEL_CODE){
							step = ConfigGet(CFG_REAGENT_CLEAN_UP_POS);
						}
						else if(work_position == REAGENT_LEVEL_CODE+1){
							step = ConfigGet(CFG_REAGENT_COOPERATE_UP_POS);
						}
						StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step, speed);
					}break;
		case MOTOR_SYN:break;					
		default:break;
	}
}
/********************************************************************
 *试剂针XY运动到工作位置
 *******************************************************************/
VOID RegeStateMvMessageHandler(Message* msg)
{		
	if((msg->id == MSG_X_DONE) || (msg->id == MSG_Y_DONE))
	{
		MainState.Parameter.count++;
		if(MainState.Parameter.count >= 2){
			MainState.Parameter.count = 0;
			RegeStateEndHandler();
		}
	}
	switch(MainState.stateManageMid)
	{
		case STATE1_MV_BASIC:{
							MainState.Parameter.wrkCode = msg->p1;						
							RegeWrkStateMessageHandler(2, MainState.Parameter.wrkCode, REAGENT_X_YLINKAGEMOTOR2_SPEED );
							TimerStart2(200, 1);							
							MainState.stateManageMid = STATE1_MV_MV;
						}break;
		case STATE1_MV_MV:{
							if(msg->id == MSG_TIMER2){
								RegeWrkStateMessageHandler(1, MainState.Parameter.wrkCode, REAGENT_X_YLINKAGEMOTOR1_SPEED );
								MainState.stateManageMid = STATE1_MV_MAX;
							}								
						}break;
		case STATE1_MV_MAX:	break;															
		default:break;
	}
}



#include "stateMoveWork.h"

//图纸上各点坐标
FLOAT GetAxisPos(INT32 wrkPos,INT32 pos_num)
{
	INT16 posx,posy;
	FLOAT pos[2];
	INT16 num = 0;
	if((wrkPos >= SAMPLE_RACK_STARTCODE) && \
	   (wrkPos < SAMPLE_RACK_STARTCODE + SAMPLE_RACK_NUM)){
		num = wrkPos - SAMPLE_RACK_STARTCODE;
		posx = num / 10;
		posy = num % 10;
		pos[0] = posx * 20.2;
		pos[1] = posy * 19;
	}
	else if((wrkPos >= SAMPLE_COOLBIT_STARTCODE) && \
		(wrkPos < SAMPLE_COOLBIT_STARTCODE + SAMPLE_COOLBIT_NUM)){
		num = wrkPos - SAMPLE_COOLBIT_STARTCODE;
		posx = 0;
		posy = num % 7;
		pos[0] = posx;
		pos[1] = posy * 28;
	}
	else if((wrkPos >= SAMPLE_ROOMTEMPERATURE_STARTCODE) && \
		    (wrkPos < SAMPLE_ROOMTEMPERATURE_STARTCODE + SAMPLE_ROOMTEMPERATURE_NUM)){
		num = wrkPos - SAMPLE_ROOMTEMPERATURE_STARTCODE;
		posx = 0;
		posy = num % 4;
		pos[0] = posx;
		pos[1] = posy * 42.5;		
	}
	else if((wrkPos >= SAMPLE_NEEDLESHORIZONTAL_STARTCODE) && \
		    (wrkPos <= SAMPLE_NEEDLESHORIZONTAL_STARTCODE + 2)){
		num = wrkPos - SAMPLE_NEEDLESHORIZONTAL_STARTCODE;
		switch(num){
			case 0:{ //清洗池
					pos[0] = ConfigGet(CFG_SAMPLE_CLEAN_X_POS);
					pos[1] = ConfigGet(CFG_SAMPLE_CLEAN_Y_POS);
				}break;
			case 1:{ //调度盘
					pos[0] = ConfigGet(CFG_SAMPLE_DISK_X_POS);
					pos[1] = ConfigGet(CFG_SAMPLE_DISK_Y_POS);
				}break;
			case 2:{ //配合抓杯手
					pos[0] = ConfigGet(CFG_SAMPLE_COOPERATE_X_POS);
					pos[1] = ConfigGet(CFG_SAMPLE_COOPERATE_Y_POS);
				}break;
			default:break;
		}
	}
	return pos[pos_num];
}

STATIC INT32 GetWorkPos(INT32 wrkPos,INT32 pos_num)
{
	FLOAT M[3][3],axis_y1,axis_x2,axis_y2;
	INT32 pos_x[3],pos_y[3],pos[2];
	if((wrkPos >= SAMPLE_RACK_STARTCODE) && \
	   (wrkPos < SAMPLE_RACK_STARTCODE + SAMPLE_RACK_NUM)){
		pos_x[0] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_X_POS1);
		pos_x[1] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_X_POS2);
		pos_x[2] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_X_POS3);
		pos_y[0] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_Y_POS1);
		pos_y[1] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_Y_POS2);
		pos_y[2] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_Y_POS3);
		
		axis_y1 = GetAxisPos(10,1);   
		axis_x2 = GetAxisPos(71,0);    
		axis_y2 = GetAxisPos(71,1);    
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = (FLOAT)(pos_x[2] - M[2][0] - axis_y2*M[1][0]) / axis_x2;
		M[0][1] = (FLOAT)(pos_y[2] - M[2][1] - axis_y2*M[1][1]) / axis_x2;		
		pos[0] = (INT32)(GetAxisPos(wrkPos,0)*M[0][0] + GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,0)*M[0][1] + GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);
//		pos[0] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_X_POS1);
//		pos[1] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_Y_POS1); 
	 }
	else if((wrkPos >= SAMPLE_COOLBIT_STARTCODE) && \
		    (wrkPos < SAMPLE_COOLBIT_STARTCODE + SAMPLE_COOLBIT_NUM)){
		pos_x[0] = ConfigGet(CFG_SAMPLE_COOL_X_POS1);
		pos_x[1] = ConfigGet(CFG_SAMPLE_COOL_X_POS2);
		pos_x[2] = 0;
		pos_y[0] = ConfigGet(CFG_SAMPLE_COOL_Y_POS1);
		pos_y[1] = ConfigGet(CFG_SAMPLE_COOL_Y_POS2);
		pos_y[2] = 0;
		axis_y1 = GetAxisPos(157,1);
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = 0;
		M[0][1] = 0;		
		pos[0] = (INT32)(GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);
//		pos[0] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_X_POS1);
//		pos[1] = ConfigGet(CFG_SAMPLE_SAMPLEAREA_Y_POS1); 
	}
	else if((wrkPos >= SAMPLE_ROOMTEMPERATURE_STARTCODE) && \
		    (wrkPos < SAMPLE_ROOMTEMPERATURE_STARTCODE + SAMPLE_ROOMTEMPERATURE_NUM)){	
		pos_x[0] = ConfigGet(CFG_SAMPLE_ROOM_X_POS1);
		pos_x[1] = ConfigGet(CFG_SAMPLE_ROOM_X_POS2);
		pos_x[2] = 0;
		pos_y[0] = ConfigGet(CFG_SAMPLE_ROOM_Y_POS1);
		pos_y[1] = ConfigGet(CFG_SAMPLE_ROOM_Y_POS2);
		pos_y[2] = 0;
				
		axis_y1 = GetAxisPos(184,1);   
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = 0;
		M[0][1] = 0;		
		pos[0] = (INT32)(GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);
//		pos[0] = ConfigGet(CFG_SAMPLE_ROOM_X_POS1);
//		pos[1] = ConfigGet(CFG_SAMPLE_ROOM_Y_POS1);
	}	 
	else if((wrkPos >= SAMPLE_NEEDLESHORIZONTAL_STARTCODE) && \
		    (wrkPos < SAMPLE_NEEDLESHORIZONTAL_STARTCODE + SAMPLE_NEEDLESHORIZONTAL_NUM)){
		pos[0] = (INT32)GetAxisPos(wrkPos,0);
		pos[1] = (INT32)GetAxisPos(wrkPos,1);		
	}
	else{
		pos[0] = pos[1] = 0;
		return (pos[pos_num]);
	}	
	return pos[pos_num];
}
/********************************************************************
 *
 *******************************************************************/
VOID SampleWrkStateMessageHandler(INT16 motor,INT32 wrkPos, UINT32 speed)
{
	INT32 step = 0;
	switch(motor - 1)
	{
		case MOTOR_X:{		
						step = GetWorkPos(wrkPos,0);
						StepMotorMovAbs (&(MainState.motors[MOTOR_X]), step, speed);
					}break;											
		case MOTOR_Y:{			
						step = GetWorkPos(wrkPos,1);
						StepMotorMovAbs (&(MainState.motors[MOTOR_Y]), step, speed);
					}break;
		case MOTOR_Z:{
						if((wrkPos >= SAMPLE_RACK_STARTCODE) && \
						   (wrkPos < SAMPLE_RACK_STARTCODE + SAMPLE_RACK_NUM)){
							step = ConfigGet(CFG_SAMPLE_SAMPLEAREA_UP_POS);  //样本架
						}
						else if((wrkPos >= SAMPLE_COOLBIT_STARTCODE) && \
								(wrkPos < SAMPLE_COOLBIT_STARTCODE + SAMPLE_COOLBIT_NUM)){
							step = ConfigGet(CFG_SAMPLE_COOL_UP_POS);		 //冷藏
						}
						else if((wrkPos >= SAMPLE_ROOMTEMPERATURE_STARTCODE) && \
								(wrkPos < SAMPLE_ROOMTEMPERATURE_STARTCODE + SAMPLE_ROOMTEMPERATURE_NUM)){
							step = ConfigGet(CFG_SAMPLE_ROOM_UP_POS);		 //常温
						}	 
						else if(wrkPos == SAMPLE_NEEDLESHORIZONTAL_STARTCODE){
							step = ConfigGet(CFG_SAMPLE_CLEAN_UP_POS);		 //清洗
						}
						else if(wrkPos == SAMPLE_NEEDLESHORIZONTAL_STARTCODE+1){
							step = ConfigGet(CFG_SAMPLE_DISK_UP_POS);	 	//调度盘	
						}
						else if(wrkPos == SAMPLE_NEEDLESHORIZONTAL_STARTCODE+2){
							step = ConfigGet(CFG_SAMPLE_COOPERATE_UP_POS);	 //配合抓手	
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
VOID SampleStateMvMessageHandler(Message* msg)
{		
//	STATIC UINT8 numcount = 0;	
	if((msg->id == MSG_X_DONE) || (msg->id == MSG_Y_DONE))
	{
		MainState.Parameter.count++;
		if(MainState.Parameter.count >= 2){
			MainState.Parameter.count = 0;
			SampleStateEndHandler();
		}
	}
	switch(MainState.stateManageMid)
	{
		case STATE1_MV_BASIC:{
							MainState.Parameter.wrkCode = msg->p1;						
							SampleWrkStateMessageHandler(2, MainState.Parameter.wrkCode, SAMPLE_X_YLINKAGEMOTOR2_SPEED );
							TimerStart2(200, 1);							
							MainState.stateManageMid = STATE1_MV_MV;
						}break;
		case STATE1_MV_MV:{
							if(msg->id == MSG_TIMER2){
								SampleWrkStateMessageHandler(1, MainState.Parameter.wrkCode, SAMPLE_X_YLINKAGEMOTOR1_SPEED );
								MainState.stateManageMid = STATE1_MV_MAX;
							}								
						}break;
		case STATE1_MV_MAX: break;							
		default:break;
	}
}




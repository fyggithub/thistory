#include "stateMoveWork.h"

//图纸上各点坐标
FLOAT GetAxisPos(INT32 wrkPos,INT16 pos_num)
{
	INT16 posx,posy;
	FLOAT pos[2];
	INT32 num;
	if((wrkPos >= GRAB_MEASUREMENTBIT_STARTCODE) && \
	   (wrkPos < GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM)){
		num = wrkPos - GRAB_MEASUREMENTBIT_STARTCODE;
		posx = 0;
		posy = num % 12;
		pos[0] = posx;
		pos[1] = posy * 15;
	}
	else if((wrkPos >= GRAB_INCUBATIONBIT_STARTCODE) && \
		    (wrkPos < GRAB_INCUBATIONBIT_STARTCODE + GRAB_INCUBATIONBIT_NUM)){
		num = wrkPos - GRAB_INCUBATIONBIT_STARTCODE;
		posx = num / 6;
		posy = num % 6;
		pos[0] = posx * 15;
		pos[1] = posy * 30;
	}
	else if((wrkPos >= GRAB_MAGNETICBEADCUP_STARTCODE) && \
		    (wrkPos < GRAB_MAGNETICBEADCUP_STARTCODE + GRAB_MAGNETICBEADCUP_NUM)){
		num = wrkPos - GRAB_MAGNETICBEADCUP_STARTCODE;
		posx = 0;
		posy = num % 12;
		pos[0] = posx;
		pos[1] = posy * 15;		
	}
	return pos[pos_num];
}

STATIC INT32 GetWorkPos(INT32 wrkPos,INT32 pos_num)
{
	FLOAT M[4][4],axis_y1,axis_x2,axis_y2;
	INT32 pos_x[3],pos_y[3],pos_z[3],pos[3];
	INT16 num;
	if((wrkPos >= GRAB_MEASUREMENTBIT_STARTCODE) && \
	   (wrkPos < GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM)){
		num = wrkPos - GRAB_MEASUREMENTBIT_STARTCODE;
		pos_x[0] = ConfigGet(CFG_GRAB_MESURE_X_POS1);
		pos_x[1] = ConfigGet(CFG_GRAB_MESURE_X_POS12);
		pos_x[2] = 0;		
		pos_y[0] = ConfigGet(CFG_GRAB_MESURE_Y_POS1);
		pos_y[1] = ConfigGet(CFG_GRAB_MESURE_Y_POS12);
		pos_y[2] = 0;
		pos_z[0] = ConfigGet(CFG_GRAB_MESURE_CATCH_POS);
		pos_z[1] = ConfigGet(CFG_GRAB_MESURE_CATCH_POS12);
		axis_y1 = GetAxisPos(12,1);  
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = 0;
		M[0][1] = 0;
		M[2][3] = (FLOAT) pos_z[0];
		M[1][3] = (FLOAT)(pos_z[1] - pos_z[0]) / axis_y1;   
		pos[0] = (INT32)(GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);	
		pos[2] = (INT32)(GetAxisPos(wrkPos,1)*M[1][3] + M[2][3]);
	}
	else if((wrkPos >= GRAB_INCUBATIONBIT_STARTCODE) && \
		    (wrkPos < GRAB_INCUBATIONBIT_STARTCODE + GRAB_INCUBATIONBIT_NUM)){
		num = wrkPos - GRAB_INCUBATIONBIT_STARTCODE;		
		pos_x[0] = ConfigGet(CFG_GRAB_BORN_X_POS21);
		pos_x[1] = ConfigGet(CFG_GRAB_BORN_X_POS26);
		pos_x[2] = ConfigGet(CFG_GRAB_BORN_X_POS38);
		pos_y[0] = ConfigGet(CFG_GRAB_BORN_Y_POS21);
		pos_y[1] = ConfigGet(CFG_GRAB_BORN_Y_POS26);
		pos_y[2] = ConfigGet(CFG_GRAB_BORN_Y_POS38);
		pos_z[0] = ConfigGet(CFG_GRAB_BORN_CATCH_POS);
		pos_z[1] = ConfigGet(CFG_GRAB_BORN_CATCH_POS26);		
		pos_z[2] = ConfigGet(CFG_GRAB_BORN_CATCH_POS38);	
		axis_y1 = GetAxisPos(26,1);    
		axis_x2 = GetAxisPos(38,0);    
		axis_y2 = GetAxisPos(38,1);    
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = (FLOAT)(pos_x[2] - M[2][0] - axis_y2*M[1][0]) / axis_x2;
		M[0][1] = (FLOAT)(pos_y[2] - M[2][1] - axis_y2*M[1][1]) / axis_x2;		
		M[2][3] = (FLOAT)pos_z[0];
		M[1][3] = (FLOAT)(pos_z[1] - pos_z[0]) / axis_y1;
		M[0][3]	= (FLOAT)(pos_z[2] - M[2][3] - axis_y2*M[1][3]) / axis_x2;	
		pos[0] = (INT32)(GetAxisPos(wrkPos,0)*M[0][0] + GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,0)*M[0][1] + GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);
		pos[2] = (INT32)(GetAxisPos(wrkPos,0)*M[0][3] + GetAxisPos(wrkPos,1)*M[1][3] + M[2][3]);
	}
	else if((wrkPos >= GRAB_MAGNETICBEADCUP_STARTCODE) && \
		    (wrkPos < GRAB_MAGNETICBEADCUP_STARTCODE + GRAB_MAGNETICBEADCUP_NUM)){
		num = wrkPos - GRAB_MAGNETICBEADCUP_STARTCODE;		
		pos_x[0] = ConfigGet(CFG_GRAB_MEGNETIC_X_POS61);
		pos_x[1] = ConfigGet(CFG_GRAB_MEGNETIC_X_POS72);
		pos_x[2] = 0;
		pos_y[0] = ConfigGet(CFG_GRAB_MEGNETIC_Y_POS61);
		pos_y[1] = ConfigGet(CFG_GRAB_MEGNETIC_Y_POS72);
		pos_y[2] = 0;
		pos_z[0] = ConfigGet(CFG_GRAB_MAGNETIC_CATCH_POS);
		pos_z[1] = ConfigGet(CFG_GRAB_MAGNETIC_CATCH_POS72);
		axis_y1 = GetAxisPos(72,1);
		M[2][0] = (FLOAT)pos_x[0];
		M[2][1] = (FLOAT)pos_y[0];
		M[1][0] = (FLOAT)(pos_x[1] - M[2][0]) / axis_y1;
		M[1][1] = (FLOAT)(pos_y[1] - M[2][1]) / axis_y1;
		M[0][0] = 0;
		M[0][1] = 0;
		M[2][3] = (FLOAT)pos_z[0];
		M[1][3] = (FLOAT)(pos_z[1] - pos_z[0]) / axis_y1;
		pos[0] = (INT32)(GetAxisPos(wrkPos,1)*M[1][0] + M[2][0]);
		pos[1] = (INT32)(GetAxisPos(wrkPos,1)*M[1][1] + M[2][1]);
		pos[2] = (INT32)(GetAxisPos(wrkPos,1)*M[1][3] + M[2][3]);
	}
	else if(wrkPos == GRAB_MAGNETICBEAD_MEASURE_STARTCODE){	//磁珠法测量位置
		pos[0] = ConfigGet(CFG_GRAB_MAGMEA_X_POS);
		pos[1] = ConfigGet(CFG_GRAB_MEGMEA_Y_POS);
		pos[2] = ConfigGet(CFG_GRAB_MAGMEA_CATCH_POS);
	}
	else if((wrkPos >= GRAB_COOPERATION_STARTCODE) && \
		    (wrkPos < GRAB_COOPERATION_STARTCODE + GRAB_COOPERATION_NUM)){
		num = wrkPos - GRAB_COOPERATION_STARTCODE;
		switch(num)
		{
			case 0:{
					pos[0] = ConfigGet(CFG_GRAB_DISK_X_POS);  //调度盘
					pos[1] = ConfigGet(CFG_GRAB_DISK_Y_POS);
					pos[2] = ConfigGet(CFG_GRAB_DISK_CATCH_POS);
				}break;
			case 1:{
					pos[0] = ConfigGet(CFG_GRAB_COOPERATE_SAMPLE_XPOS);  //配合样本针
					pos[1] = ConfigGet(CFG_GRAB_COOPERATE_SAMPLE_YPOS);
					pos[2] = ConfigGet(CFG_GRAB_COOPERATE_SAMPLE_VERTICAL);
				}break;
			case 2:{
					pos[0] = ConfigGet(CFG_GRAB_COOPERATE_REAGENT_XPOS); //配合试剂针
					pos[1] = ConfigGet(CFG_GRAB_COOPERATE_REAGENT_YPOS);
					pos[2] = ConfigGet(CFG_GRAB_COOPERATE_REAGENT_VERTICAL);
				}break;
			default:break;
		}
	}
	else if((wrkPos >= GRAB_THROWCUP_STARTCODE) && \
			(wrkPos < GRAB_THROWCUP_STARTCODE + GRAB_THROWCUP_NUM)){
		num = wrkPos - GRAB_THROWCUP_STARTCODE;
		switch(num)
		{
			case 0:{
					pos[0] = ConfigGet(CFG_GRAB_THROW_X_POS1);  //抛杯1
					pos[1] = ConfigGet(CFG_GRAB_THROW_Y_POS1);
					pos[2] = ConfigGet(CFG_GRAB_THROW_VERTICAL);
				}break;
			case 1:{
					pos[0] = ConfigGet(CFG_GRAB_THROW_X_POS2);  //抛杯2
					pos[1] = ConfigGet(CFG_GRAB_THROW_Y_POS2);
					pos[2] = ConfigGet(CFG_GRAB_THROW_VERTICAL);
				}break;
			default:break;
		}
	}
	else{
		pos[0] = pos[1] = 0;
		return (pos[pos_num]);
	}	
	
	return pos[pos_num];
}

VOID GrabWrkStateMessageHandler(INT16 motor,INT32 wrkPos, UINT32 speed,UINT8 type)
{
	INT32 step = 0;
	switch(motor - 1)
	{
		case 0:{	//X轴			
						step = GetWorkPos(wrkPos,0);
						StepMotorMovAbs (&(MainState.motors[MOTOR_X]), step, speed);
					}break;											
		case 1:{   //Y轴
						step = GetWorkPos(wrkPos,1);
						if(type == 1){
							step = step - ConfigGet(CFG_GRAB_CATCH_Y_DISTANCE);       //参数37;
						}
						StepMotorMovAbs (&(MainState.motors[MOTOR_Y]), step, speed);
					}break;
		case 2:{    //Z轴
						step = GetWorkPos(wrkPos,2);
//						if((wrkPos >= GRAB_MEASUREMENTBIT_STARTCODE) && \
//								(wrkPos < GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM)){  //光学测量位置
//							step = ConfigGet(CFG_GRAB_MESURE_CATCH_POS);
//						}
//						else if((wrkPos >= GRAB_INCUBATIONBIT_STARTCODE) && \
//								(wrkPos < GRAB_INCUBATIONBIT_STARTCODE + GRAB_INCUBATIONBIT_NUM)){    //孵育位置
//							step = ConfigGet(CFG_GRAB_BORN_CATCH_POS);
//						}
//						else if((wrkPos >= GRAB_MAGNETICBEADCUP_STARTCODE) && \
//								(wrkPos < GRAB_MAGNETICBEADCUP_STARTCODE + GRAB_MAGNETICBEADCUP_NUM)){  //磁珠杯测量位置
//							step = ConfigGet(CFG_GRAB_MAGNETIC_CATCH_POS);
//						}
//						else if(wrkPos == GRAB_MAGNETICBEAD_MEASURE_STARTCODE){     //磁珠法测量位置
//							step = ConfigGet(CFG_GRAB_MAGMEA_CATCH_POS);
//						}
//						else if((wrkPos >= GRAB_COOPERATION_STARTCODE) && \
//								(wrkPos < GRAB_COOPERATION_STARTCODE + GRAB_COOPERATION_NUM)){
//							num = wrkPos - GRAB_COOPERATION_STARTCODE;
//							switch(num)
//							{
//								case 0:step = ConfigGet(CFG_GRAB_DISK_CATCH_POS);			 break;  //调度盘	
//								case 1:step = ConfigGet(CFG_GRAB_COOPERATE_SAMPLE_VERTICAL); break;  //配合样本针								
//								case 2:step = ConfigGet(CFG_GRAB_COOPERATE_REAGENT_VERTICAL);break;  //配合试剂针	
//								default:break;
//							}
//						}
//						else if((wrkPos >= GRAB_THROWCUP_STARTCODE) && \
//								(wrkPos < GRAB_THROWCUP_STARTCODE + GRAB_THROWCUP_NUM)){
//							step = ConfigGet(CFG_GRAB_THROW_VERTICAL);
//						}
						StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step, speed);
					}break;
		default:break;
	}
}
/********************************************************************
 *抓手XY运动到工作位置
 *******************************************************************/
VOID GrabStateMvMessageHandler(Message* msg)
{		
//	STATIC UINT8 numcount = 0;
	UINT8 align_hole = 0;
	
	if((msg->id == MSG_X_DONE) || (msg->id == MSG_Y_DONE))
	{
		MainState.Parameter.count++;
		if(MainState.Parameter.count >= 2){
			MainState.Parameter.count = 0;
			GrabStateEndHandler();
		}
	}
	switch(MainState.stateManageMid)
	{
		case STATE1_MV_BASIC:{
							MainState.Parameter.wrkCode = msg->p1;
							MainState.Parameter.alignhole = msg->p2;
							GrabWrkStateMessageHandler(1, MainState.Parameter.wrkCode, GRAB_X_YLINKAGEMOTOR1_SPEED,0);							
							TimerStart2(80, 1);							
							MainState.stateManageMid = STATE1_MV_MV;
						}break;
		case STATE1_MV_MV:{
							if(msg->id == MSG_TIMER2){		//直接走到孔位
								align_hole = MainState.Parameter.alignhole;
								GrabWrkStateMessageHandler(2, MainState.Parameter.wrkCode, GRAB_X_YLINKAGEMOTOR2_SPEED,align_hole);
								MainState.stateManageMid = STATE1_MV_MAX;
							}								
						}break;
		case STATE1_MV_MAX:	break;						
		default:break;
	}
}
/********************************************************************
 *Z轴运行到指定垂直高度
 *******************************************************************/
VOID ZAxisSpecifyVerticalPos(Message* msg)
{
	INT32 step = 0,wrkPos = 0;
	MainState.Parameter.wrkCode = msg->p1;
	MainState.Parameter.wrkPos = msg->p2;
	wrkPos = MainState.Parameter.wrkCode;
	switch(MainState.Parameter.wrkPos)
	{
		case 1:	step = ConfigGet(CFG_GRAB_POS_VRST); break;//初始高度位置
		case 2:{	//抓杯高度位置
					step = GetWorkPos(wrkPos,2);
//					if((MainState.Parameter.wrkCode >= GRAB_MEASUREMENTBIT_STARTCODE) && \
//						(MainState.Parameter.wrkCode < GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM)){  
//						step = ConfigGet(CFG_GRAB_MESURE_CATCH_POS);	//光学测量垂直位置
//					}
//					else if((wrkPos >= GRAB_INCUBATIONBIT_STARTCODE) && \
//							(wrkPos < GRAB_INCUBATIONBIT_STARTCODE + GRAB_INCUBATIONBIT_NUM)){  
//						step = ConfigGet(CFG_GRAB_BORN_CATCH_POS);		//光学孵育垂直位置
//					}
//					else if((wrkPos >= GRAB_MAGNETICBEADCUP_STARTCODE) && \
//							(wrkPos < GRAB_MAGNETICBEADCUP_STARTCODE + GRAB_MAGNETICBEADCUP_NUM)){ 
//						step = ConfigGet(CFG_GRAB_MAGNETIC_CATCH_POS);	//磁珠杯垂直位置
//					}
//					else if(wrkPos == GRAB_MAGNETICBEAD_MEASURE_STARTCODE){     
//						
//						step = ConfigGet(CFG_GRAB_MAGMEA_CATCH_POS);	//磁珠法测量位置
//					}
//					else if(wrkPos == GRAB_COOPERATION_STARTCODE){						
//						step = ConfigGet(CFG_GRAB_DISK_CATCH_POS);		//调度盘					
//					}									
				}break;
		case 3:{	//放杯高度位置
					if((MainState.Parameter.wrkCode >= GRAB_MEASUREMENTBIT_STARTCODE) && \
						(MainState.Parameter.wrkCode < GRAB_MEASUREMENTBIT_STARTCODE + GRAB_MEASUREMENTBIT_NUM)){  
						step = ConfigGet(CFG_GRAB_MESURE_CATCH_POS) - ConfigGet(CFG_GRAB_DELTA_HIGHT);	//光学测量垂直位置
					}
					else if((wrkPos >= GRAB_INCUBATIONBIT_STARTCODE) && \
							(wrkPos < GRAB_INCUBATIONBIT_STARTCODE + GRAB_INCUBATIONBIT_NUM)){  
						step = ConfigGet(CFG_GRAB_BORN_CATCH_POS) - ConfigGet(CFG_GRAB_DELTA_HIGHT);		//光学孵育垂直位置
					}
					else if((wrkPos >= GRAB_MAGNETICBEADCUP_STARTCODE) && \
							(wrkPos < GRAB_MAGNETICBEADCUP_STARTCODE + GRAB_MAGNETICBEADCUP_NUM)){ 
						step = ConfigGet(CFG_GRAB_MAGNETIC_CATCH_POS) - ConfigGet(CFG_GRAB_DELTA_HIGHT);	//磁珠杯垂直位置
					}
					else if(wrkPos == GRAB_MAGNETICBEAD_MEASURE_STARTCODE){     
						step = ConfigGet(CFG_GRAB_MAGMEA_CATCH_POS) - ConfigGet(CFG_GRAB_DELTA_HIGHT);	//磁珠法测量位置
					}
					else if(wrkPos == GRAB_COOPERATION_STARTCODE){						
						step = ConfigGet(CFG_GRAB_DISK_CATCH_POS) - ConfigGet(CFG_GRAB_DELTA_HIGHT);		//调度盘					
					}
			  }break;
		case 4:{	//吸排液高度位置
					if(wrkPos == GRAB_COOPERATION_STARTCODE+1)
						step = ConfigGet(CFG_GRAB_COOPERATE_SAMPLE_VERTICAL);		//配合样本针垂直位置	
					else if(wrkPos == GRAB_COOPERATION_STARTCODE+2)
						step = ConfigGet(CFG_GRAB_COOPERATE_REAGENT_VERTICAL);		//配合试剂针垂直位置	
			  }break;
		case 5:step = ConfigGet(CFG_GRAB_MIX_HIGHT);break;		//混匀高度位置
		case 6:{	//抛杯位置
					if((wrkPos >= GRAB_THROWCUP_STARTCODE) && \
							(wrkPos < GRAB_THROWCUP_STARTCODE + GRAB_THROWCUP_NUM)){
						step = ConfigGet(CFG_GRAB_THROW_VERTICAL);
					}
			  }break;
		default:break;
	}
	switch(MainState.stateManageMid)
	{
		case STATE1_ZAXIS_MV:{
					StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), step, ZAXIS_POSITION_SPEED);
					MainState.stateManageMid = STATE1_ZAXIS_MAX;
				}break;
		case STATE1_ZAXIS_MAX:{
					if(msg->id == MSG_Z_DONE){
						GrabStateEndHandler();   //状态切换及数据发送
					}						
				}break;
		default:break;		
	}	
}








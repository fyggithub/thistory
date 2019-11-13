#include "stateMoveWork.h"

////��Ҫ��е�ṩͼֽ���� 2#��еͼֽ
//CONST FLOAT AxisPos[22][2] = {   			//ͼֽ�ϸ�������
//	{80.6,2.8},            //�����1��λ��
//	{80.6,43.7},           //�����2��λ��
//	{35,0},           	   //�п���1��λ��
//	{0,0},                 //�п���2��λ��
//	{35,34},               //�п���3��λ��
//	{0,34},                //�п���4��λ��
//	{35,68},               //�п���5��λ��
//	{0,68},                //�п���6��λ��
//	{35,102},              //�п���7��λ��
//	{0,102},               //�п���8��λ��
//	{35,136},              //�п���9��λ��
//	{0,136},               //�п���10��λ��
//	{93.6,80},             //С����1��λ��
//	{67.6,80},             //С����2��λ��
//	{93.6,106.5},          //С����3��λ��
//	{67.6,106.5},          //С����4��λ��
//	{93.6,133},            //С����5��λ��
//	{67.6,133},            //С����6��λ��
//	{93.6,159.5},          //С����7��λ��
//	{67.6,159.5},          //С����8��λ��
//	{41.5,164.5},          //С����9��λ��
//	{15.5,164.5}           //С����10��λ��
//};

//��Ҫ��е�ṩͼֽ���� 3#��еͼֽ
CONST FLOAT AxisPos[28][2] = {   			//ͼֽ�ϸ�������
	{115.3,0},            	//�����1��λ��
	{115.3,46.5},           //�����2��λ��
	{70,0},           	   	//�п���1��λ��
	{35,0},                 //�п���2��λ��
	{0,0},               	//�п���3��λ��
	{70,35},                //�п���4��λ��
	{35,35},               	//�п���5��λ��
	{0,35},                	//�п���6��λ��
	{70,70},              	//�п���7��λ��
	{35,70},               	//�п���8��λ��
	{0,70},              	//�п���9��λ��
	{70,105},               //�п���10��λ��
	{35,105},               //�п���11��λ��
	{0,105},               	//�п���12��λ��
	{129.3,80.5},           //С����1��λ��
	{101.3,80.5},           //С����2��λ��
	{129.3,108.5},          //С����3��λ��
	{101.3,108.5},          //С����4��λ��
	{129.3,136.5},          //С����5��λ��
	{101.3,136.5},          //С����6��λ��
	{129.3,164.5},          //С����7��λ��
	{101.3,164.5},          //С����8��λ��
	{70,136.5},          	//С����9��λ��
	{35,136.5},           	//С����10��λ��
	{0,136.5},           	//С����11��λ��
	{70,164.5},           	//С����12��λ��
	{35,164.5},           	//С����13��λ��
	{0,164.5}           	//С����14��λ��
};

////��Ҫ��е�ṩͼֽ���� 1#��еͼֽ
//CONST FLOAT AxisPos[22][2] = {   			//ͼֽ�ϸ�������
//	{0,0},            //�����1��λ��
//	{0,0},           //�����2��λ��
//	{0,136},           	   //�п���1��λ��
//	{0,0},                 //�п���2��λ��
//	{0,136},               //�п���3��λ��
//	{0,136},                //�п���4��λ��
//	{0,136},               //�п���5��λ��
//	{0,136},                //�п���6��λ��
//	{0,136},              //�п���7��λ��
//	{0,136},               //�п���8��λ��
//	{0,136},              //�п���9��λ��
//	{0,136},               //�п���10��λ��
//	{93.6,80},             //С����1��λ��
//	{93.6,80},             //С����2��λ��
//	{93.6,80},          	//С����3��λ��
//	{93.6,80},          	//С����4��λ��
//	{93.6,80},            //С����5��λ��
//	{93.6,80},            //С����6��λ��
//	{93.6,80},          //С����7��λ��
//	{93.6,80},          //С����8��λ��
//	{93.6,80},          //С����9��λ��
//	{93.6,80},           //С����10��λ��
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
		axis_y1 = AxisPos[13][1];    //ͼֽ��y1����  ���п�12��λ��
		axis_x2 = AxisPos[14][0];    //ͼֽ��x2����  ��С��1��λ��
		axis_y2 = AxisPos[14][1];    //ͼֽ��y2����  ��С��1��λ��
		
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
 *�Լ���XY�˶�������λ��
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



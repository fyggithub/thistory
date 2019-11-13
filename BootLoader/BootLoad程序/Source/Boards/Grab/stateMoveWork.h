#ifndef __stateMoveWork_H__
#define __stateMoveWork_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "Grab.h"
#include "SendResultCode.h" 
#include "stateReady.h"
#include "ConfigGrab.h"
#include "StepMotorGrab.h"
//#define POSTION_ADDPLUSE     148    //�ڳ�ʼλ���ڼ���250�����壬��Ҫ��Z��̧��λ��

#define GRAB_MEASUREMENTBIT_STARTCODE               1      //ˮƽλ�ù�ѧ����λ����ʼ����
#define GRAB_MEASUREMENTBIT_NUM                     12     //ˮƽλ�ù�ѧ����λ�ò�������
#define GRAB_INCUBATIONBIT_STARTCODE                21     //ˮƽλ�÷���λ��ʼ����
#define GRAB_INCUBATIONBIT_NUM                      18     //ˮƽλ�÷�����������
#define GRAB_MAGNETICBEADCUP_STARTCODE              61     //ˮƽλ�ô����ֶ��ű���ʼ���        
#define GRAB_MAGNETICBEADCUP_NUM                    12      //ˮƽλ�ô����ֶ��ű���������
#define GRAB_MAGNETICBEAD_MEASURE_STARTCODE         51     //ˮƽλ�ô��鷨����λ����ʼ����
#define GRAB_MAGNETICBEAD_MEASURE_NUM               1      //ˮƽλ�ô��鷨����λ�ò�������
#define GRAB_COOPERATION_STARTCODE                  101    //ˮƽλ��ץ���������ʼ���
#define GRAB_COOPERATION_NUM                        3      //ˮƽλ��ץ������ϲ�������
#define GRAB_THROWCUP_STARTCODE                     111    //ˮƽλ���ױ�λ����ʼ���
#define GRAB_THROWCUP_NUM                           2      //ˮƽλ���ױ�λ�ò�������
#define GRAB_EXTREMEPOSITION_STARTCODE              204    //ˮƽλ�ü���λ����ʼ���
#define GRAB_EXTREMEPOSITION_NUM                    4      //ˮƽλ�ü���λ�ò�������

enum {
	STATE1_MV_BASIC = 0, STATE1_MV_MV, STATE1_MV_MAX,
};

enum{
	STATE1_ZAXIS_BASIC = 0,STATE1_ZAXIS_MV,STATE1_ZAXIS_MAX
};

VOID GrabWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed,UINT8 type);
VOID GrabStateMvMessageHandler(Message* msg);
VOID ZAxisSpecifyVerticalPos(Message* msg);

#endif



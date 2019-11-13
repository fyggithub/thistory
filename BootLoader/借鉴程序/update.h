
#ifndef __UPDATE_H__
#define __UPDATE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "CoreTypes.h"

//��������Ĭ�ϳ���
#define FILE_LEN_ADDR_DEFAULT			(0x1FF000)		//Flash������2M�е����һ������4096��ʼλ��		
#define	FILE_START_ADDR_DEFAULT			(0)				//����洢��Flash��ʼ��
//�û�������������
#define FILE_LEN_ADDR_USER				(0x1FFF000)		//Flash���������2M�е����һ������4096��ʼλ��		
#define	FILE_START_ADDR_USER			(0x1E00000)		//����洢��Flash��ʼ��	
		
#define UPGRATE_FLAG_ADDR				(0x1FFF004)		//Flash���������2M�е����һ������offset 4�ֽ�λ��	
#define UPGRATE_FLAG_VALUE				(0xccee55aa)	//�����û�����Ӧ�ó�����	
#define	UPGRATE_FLAG_CLEAR				(0xFFFFFFFF)
	
#define UPGRATE_RESULT_ADDR				(0)				//���������Ǳ����ڱ���SRAM����ʼ��ַ������СΪһ���ֽ�

#define EEPROM_FLAG_ADDR				(0)				//EEPROM�б���������ñ�ǵĵ�ַ	
	
//����������ֵ
enum{
	UPGRATE_INIT = 0,
	UPGRATE_OK=1,
	UPGRATE_CRC_FAIL,
	UPGRATE_EEPROM_FAIL,
	UPGRATE_FACTORY_OK,
	UPGRATE_USER_OK,
};	
	
BOOL SetUpgrateResultAndFlag(UINT32 Flag,UINT8 Result);	
BOOL CheckIfUpgrateFactoryCode(VOID);	
BOOL CheckUpgrateFileCrc(VOID);
BOOL CheckIfNeedUpgrateFile(VOID);	
BOOL StartUpgrateProcess(VOID);	
VOID JumpToRunUserCode(VOID);
BOOL GetUpgrateFileLen(UINT32 * FileLen);
BOOL CheckIfRestoreFactoryKeyPressed(VOID);
BOOL CheckIfUpgrateFromSd(VOID);
BOOL CheckIfCopyFactotyFileToFlash(VOID);
#ifdef __cplusplus
}
#endif
#endif //__UPDATE_H__


#include "Boot.h"

STATIC UINT32 flash_crc32;
UINT16 copybuf[PER_PAGE_ADDR_SPACE/2];
UINT16 p[1024];
UINT32 codeLen = 0;

CRC_HandleTypeDef crcProcess;
VOID CRC32_INIT(VOID)
{
	crcProcess.Instance = CRC;
	__HAL_RCC_CRC_CLK_ENABLE();
	HAL_CRC_Init(&crcProcess);
}
/********************************************************************
 *������ת���������ж�
 *******************************************************************/
STATIC VOID JumpTORunUserCode(VOID)
{
	UINT32 jumpaddress = 0;
	pFunction Jump_To_Application;
	jumpaddress = *(__IO UINT32*)(FLASH_RUN_START_ADDR + FLASH_OFFSET + 4);
	Jump_To_Application = (pFunction)jumpaddress;
	__set_MSP(*(__IO UINT32*)(FLASH_RUN_START_ADDR + FLASH_OFFSET));
	Jump_To_Application();
}
/********************************************************************
 *��16bit
 *******************************************************************/
STATIC UINT16 FlashReadHalfWord(UINT32 addr)
{
	return *(__IO UINT16*)addr;
}
/********************************************************************
 *��ȡ����ֽ�
 *******************************************************************/
STATIC VOID FlashReadMoreData(UINT32 statraddr,UINT16 *readBuf,UINT16 count)
{
	for(UINT16 i = 0;i < count;i++){
		readBuf[i] = FlashReadHalfWord(statraddr);
		statraddr += 2;
	}
}
/********************************************************************
 *д��һ���ֽ�
 *******************************************************************/
VOID WriteFlashWordNoCheck(UINT32 addr,UINT16 *buf,UINT16 num)
{		
	for(UINT16 i = 0;i < num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, buf[i]);		
		addr += 2;
	}
}
/********************************************************************
 *�����������������ο��������ж�
 *******************************************************************/
STATIC VOID FlashWrite(UINT32 addr,UINT16 *buf, UINT16 num)
{	
	UINT32 offaddr = 0,pageerror = 0;
	UINT32 pageoff = 0,pagemain = 0,pageaddr;
	FLASH_EraseInitTypeDef f;
	UINT16 i;
	
	if((addr < FLASH_BASE) || (addr >= FLASH_BANK1_END))
		return;
		 
	HAL_FLASH_Unlock();	
	offaddr  = addr - FLASH_BASE;
	pageaddr = offaddr / PER_PAGE_ADDR_SPACE;
	pageoff  = (offaddr % PER_PAGE_ADDR_SPACE)/2;
	pagemain = PER_PAGE_ADDR_SPACE/2 - pageoff;
	
	if(num <= pagemain){
		pagemain = num;
	}
	
	while(1){
		UINT32 addr0 = FLASH_BASE + pageaddr * PER_PAGE_ADDR_SPACE;
		FlashReadMoreData(addr0,copybuf,PER_PAGE_ADDR_SPACE/2);
		for(i = 0;i < pagemain;i++){
			if(copybuf[pageoff + i] != 0xFFFF) break;
		}
		
		if(i < pagemain){
			f.TypeErase   = FLASH_TYPEERASE_PAGES;
			f.PageAddress = addr0;
			f.NbPages     = 1;
			HAL_FLASHEx_Erase(&f, &pageerror);
			for(i = 0;i < pagemain;i++){
				copybuf[i + pageoff] = buf[i];
			}
			WriteFlashWordNoCheck(addr0,copybuf,PER_PAGE_ADDR_SPACE/2);
		}
		else{
			WriteFlashWordNoCheck(addr,buf,pagemain);
		}
		
		if(num == pagemain){
			break;
		}
		else{
			pageaddr++;
			pageoff++;
			buf += pagemain;
			addr += pagemain;
			num -= pagemain;
			if(num > (PER_PAGE_ADDR_SPACE/2)){
				pagemain = PER_PAGE_ADDR_SPACE/2;
			}
			else{
				pagemain = num;
			}
		}
	}
	HAL_FLASH_Lock();
}
/****************************************************************************
 *�Ӵ���ĵ�ַ��ֵ��FLASH_BKP_START_ADDR(0x08024000),
 *pages - �����ҳ��
 ****************************************************************************/
//STATIC VOID TestFlash(UINT32 addr,UINT32 pages)
//{
//	FLASH_EraseInitTypeDef f;
//	UINT32 pageerror = 0;
//	UINT16 i;
//	
//	if((addr < FLASH_BASE) || (addr >= FLASH_BANK1_END))
//		return;
//	
//	HAL_FLASH_Unlock();		
//	UINT32 addr0 = FLASH_BKP_START_ADDR;
//	
//	FlashReadMoreData(addr,copybuf,PER_PAGE_ADDR_SPACE/2);
//	for(i = 0;i < PER_PAGE_ADDR_SPACE/2;i++){
//			if(copybuf[i] != 0xFFFF) break;
//	}
//	if(i < PER_PAGE_ADDR_SPACE/2){
//		f.TypeErase   = FLASH_TYPEERASE_PAGES;
//		f.PageAddress = addr0;
//		f.NbPages     = pages;	
//		HAL_FLASHEx_Erase(&f, &pageerror);
//	}
//	for(i = 0;i < pages;i++){
//		FlashReadMoreData(FLASH_RUN_START_ADDR+i*PER_PAGE_ADDR_SPACE,copybuf,PER_PAGE_ADDR_SPACE/2);
//		WriteFlashWordNoCheck(addr0+i*PER_PAGE_ADDR_SPACE,copybuf,PER_PAGE_ADDR_SPACE/2);
//	}
//	HAL_FLASH_Lock();	
//}
/**************************************************************************************
 *����ʼ����������PAGE_NUMҳ������FLASH_BKP_START_ADDR��ʼ������FLASH_RUN_START_ADDR
 *len - ��Ҫ��������ĳ���
 **************************************************************************************/
STATIC VOID UpdataRunFlash(UINT32 len)
{
	UINT32 page = 0;
	page = len / PER_PAGE_ADDR_SPACE;
	
	if(page > PAGE_NUM)
		return;
	for(UINT16 i = 0;i <= page;i++){
		FlashReadMoreData(FLASH_BKP_START_ADDR+i*PER_PAGE_ADDR_SPACE,p,PER_PAGE_ADDR_SPACE/2);
		FlashWrite(FLASH_RUN_START_ADDR+i*PER_PAGE_ADDR_SPACE,p, PER_PAGE_ADDR_SPACE/2);
	}
}
/********************************************************************
 *�ж�CRC�Ƿ�ɹ�
 *******************************************************************/
STATIC UINT16 BootLoadCRC(VOID)
{
	DWORD dat0;	
	UINT16 hword,lword,dat1,dat2,runcrc1[2];
	UINT32 runcrc0;
	lword = FlashReadHalfWord(FLASH_RUN_START_ADDR);
	hword = FlashReadHalfWord(FLASH_RUN_START_ADDR + 2);	
	runcrc0  = MAKE_DWORD(hword,lword);
	
	for(UINT32 i = LEN_OFFSET;i < codeLen;i += 4)
	{	
		dat1 = FlashReadHalfWord(FLASH_RUN_START_ADDR+i);
		dat2 = FlashReadHalfWord(FLASH_RUN_START_ADDR+2+i);
		dat0 = MAKE_DWORD(dat2,dat1);
		if(i == LEN_OFFSET){
			flash_crc32 = HAL_CRC_Calculate(&crcProcess, &dat0, 1);
		}
		else{
			flash_crc32 = HAL_CRC_Accumulate(&crcProcess, &dat0, 1);	
		}				
	}
	if(flash_crc32 == runcrc0){
		runcrc1[0]  = LWORD(flash_crc32);
		runcrc1[1]  = HWORD(flash_crc32);		
		FlashWrite(FLASH_RUN_START_ADDR + 4,runcrc1, 2);
		return TRUE;
	}	
	return FALSE;
}
/********************************************************************
 *�����λ
 *******************************************************************/
STATIC VOID SoftWare(VOID)
{
	NVIC_SystemReset();
}
/********************************************************************
 *
 *******************************************************************/
VOID BootLoadRun(VOID)
{	
	UINT16 hword,lword;
	UINT32 version0,version1,crc1;    

	lword = FlashReadHalfWord(FLASH_RUN_START_ADDR + 4);
	hword = FlashReadHalfWord(FLASH_RUN_START_ADDR + 6);	
	crc1 = MAKE_DWORD(hword,lword);
	lword = FlashReadHalfWord(FLASH_RUN_START_ADDR + 12);
	hword = FlashReadHalfWord(FLASH_RUN_START_ADDR + 14);	
	version0 = MAKE_DWORD(hword,lword);
	lword = FlashReadHalfWord(FLASH_BKP_START_ADDR + 8);
	hword = FlashReadHalfWord(FLASH_BKP_START_ADDR + 10);
	codeLen  = MAKE_DWORD(hword,lword);						//�����Ǵ���ʼ��ַ��ʼ���������������CRC���ڵ�
	lword = FlashReadHalfWord(FLASH_BKP_START_ADDR + 12);
	hword = FlashReadHalfWord(FLASH_BKP_START_ADDR + 14);	
	version1 = MAKE_DWORD(hword,lword);
	
	if(version0 == version1){									//��������
		if((BootLoadCRC() == TRUE) || (crc1 == 0xffffffff)){
			JumpTORunUserCode();
		}
	}
	else if((version0 < version1) && (version1 != 0xffffffff)){ //����
		UpdataRunFlash(codeLen + LEN_OFFSET);
		if(BootLoadCRC() == TRUE){							//�����ɹ�
			SoftWare();											//��λ
		}
	}
	else{										//�쳣	
		JumpTORunUserCode();
	}
}













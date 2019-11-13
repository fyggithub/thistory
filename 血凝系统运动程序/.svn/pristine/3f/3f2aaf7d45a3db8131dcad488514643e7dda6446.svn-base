/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * AlgorithmPID.c
 *******************************************************************/
#include "AlgorithmPID.h"
#include "Board.h"
#define KIOLIMIT 0.5
#define KIOLIMITREAG 0.1//niu
#define ABS(ek) ((ek)>=0?(ek):-(ek))
/********************************************************************
 *
 *******************************************************************/
VOID   PIDInit(AlgorithmPid* pid)
{
	if(pid){
		pid->ek_1   = 0;
		pid->sum_ek = 0;
	}
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE PIDCalc(AlgorithmPid* pid, DOUBLE ek)
{
	if(pid){
//		pid->sum_ek += ek;//niu
		pid->pitem = (pid->cKp) * ek;
//		pid->iitem = (pid->cKi) * (pid->sum_ek);//niu
		pid->ditem = (pid->cKd) * (ek - (pid->ek_1));
		pid->uk = pid->pitem + pid->ditem;
		pid->ek_1 = ek;

		DOUBLE beta = 1.0;
		if (ABS(ek) > (pid->cEps1))
			beta = 0.0;
		else if (ABS(ek) < (pid->cEps2))
			beta = 1.0;
		else{
			if((pid->cEps1) == (pid->cEps2)){
				beta = 1;
			}
			else
				beta = ((pid->cEps1)-ek)/((pid->cEps1)-(pid->cEps2));
		}

		
		pid->iitem += beta * (pid->cKi) * ek; //ลฃถู
		DOUBLE KiOLimitReag = KIOLIMITREAG; //niu
		if(ABS(pid->iitem) > KiOLimitReag)//niu
			pid->iitem = 0;//niu
		else//niu
			__nop();//niu
		
#ifdef _COOL_PID
		DOUBLE KiOLimit=KIOLIMIT;		//niu
       // if((beta * pid->iitem) > KiOLimit)		//niu
		if(pid->iitem > KiOLimit)		//niu
			pid->iitem = 0;		//niu
			//__nop();		//niu
		else		//niu
			pid->uk += pid->iitem;		//niu
#else
		pid->uk += pid->iitem;
#endif
		DOUBLE uk = pid->uk;
		if (uk >= (pid->cUmax))
			uk = (pid->cUmax);
		if (uk <= (pid->cUmin))
			uk = (pid->cUmin);

		return uk;
	}
	return 0.0;
}
/********************************************************************
 *
 *******************************************************************/



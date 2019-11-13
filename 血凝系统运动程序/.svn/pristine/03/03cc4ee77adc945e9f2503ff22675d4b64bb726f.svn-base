/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * AlgorithmPID.h
 *******************************************************************/
#ifndef _ALGORITHM_PID_H_
#define _ALGORITHM_PID_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
/********************************************************************
 *
 *  uk = kp * ek + beta * ki * sum(ek) + kd * delta(ek)
 *
 *  sum(ek) = 0, sum(ek) > KiOLimit
 *  		   /  0,                    |ek| > eps1
 *         |
 *  beta = |  (eps1-ek)/(eps1-ep2),  eps2 <= |ek| <= eps1
 *         |
 *          \  1,                    |ek| < eps2
 *
 *       /  umax,   uk >= umax
 *  u = |   uk,     umin <= uk <= umax
 *       \  umin,   uk <= umin
 *******************************************************************/
typedef struct _AlgorithmPid{
	DOUBLE cKp;
	DOUBLE cKi;
	DOUBLE cKd;
	DOUBLE cUmax;
	DOUBLE cUmin;
	DOUBLE cEps1;
	DOUBLE cEps2;
	DOUBLE ek_1;
	DOUBLE sum_ek;
	DOUBLE pitem;
	DOUBLE iitem;
	DOUBLE ditem;
	DOUBLE uk;
}AlgorithmPid;
/********************************************************************
 *
 *******************************************************************/
VOID   PIDInit(AlgorithmPid* pid);
DOUBLE PIDCalc(AlgorithmPid* pid, DOUBLE ek);
/********************************************************************
 *
 *******************************************************************/
#endif // _ALGORITHM_PID_H_


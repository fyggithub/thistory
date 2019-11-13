#ifndef __HostGrab_H__
#define __HostGrab_H__


#include "Host.h"
#include "Console.h"
#include "Message.h"
#include "Datatypes.h"
#include "Board.h"
#include "Grab.h"
/********************************************************************
 *
 *******************************************************************/
#define HOST_ADDR_GRAB	(0x0A)

#define GRABHOST_CMD_VERSION			 (0x50)
#define GRABHOST_CMD_GETINFO			 (0x51)
#define GRABHOST_CMD_SETINFO			 (0x52)
#define GRABHOST_CMD_PARAINFO			 (0x53)
#define GRABHOST_CMD_PARASET			 (0x54)
#define GRABHOST_CMD_WRKSAVE             (0x55)
#define GRABHOST_CMD_MOTOR_ENABLE		 (0x60)
#define GRABHOST_CMD_MOTOR_RESET		 (0x61)
#define GRABHOST_CMD_MOTOR_TOZERO		 (0x62)
#define GRABHOST_CMD_MOTOR_MOVREL		 (0x63)
#define GRABHOST_CMD_MOTOR_MOVABS		 (0x64)
#define GRABHOST_CMD_MOTOR_MOVWRK		 (0x65)
#define GRABHOST_CMD_MOTOR_INQUIRE  	 (0x66)
#define GRABHOST_CMD_MIXMOTOR_CTRL  	 (0x67)
#define GRABHOST_CMD_MIXMOTOR_INQUIRE  	 (0x68)
#define GRABHOST_CMD_MOTOR_MOVXY    	 (0x69)
#define GRABHOST_CMD_ZAXISMOTOR_MOVWEK   (0x6A)
#define GRABHOST_CMD_CATCH_CUP   	 	 (0x6B)
#define GRABHOST_CMD_PUT_CUP   	 		 (0x6C)
#define GRABHOST_CMD_MIX   	 		 	 (0x6D)
#define GRABHOST_CMD_XY_MIX   	 		 (0x6E)
#define GRABHOST_CMD_THROW_CUP   	 	 (0x6F)
#define GRABHOST_CMD_PUMPSET    		 (0x58)
#define GRABHOST_CMD_PUMPGET    		 (0x59)
#define GRABHOST_CMD_VALVESET   		 (0x5A)
#define GRABHOST_CMD_VALVEGET   		 (0x5B)
#define GRABHOST_CMD_IOSET			 	 (0x5C)
#define GRABHOST_CMD_IOGET			 	 (0x5D)
#define GRABHOST_CMD_OPTOGET			 (0x5E)
#define GRABHOST_CMD_FAULT	    	 	 (0xF0)
#define GRABHOST_CMD_DEALY				 (0x0D)
#define GRABHOST_CMD_STOP	    		 (0x0E)
/********************************************************************
 *
 *******************************************************************/

VOID HostGrabInit(VOID);


/********************************************************************
 *
 *******************************************************************/


#endif



/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * 
 * Bits.h
 *******************************************************************/
#ifndef _BITS_H_
#define _BITS_H_
/********************************************************************
 * Bit op
 *******************************************************************/
#define BIT(n)		(1U<<(n))

#define BCLR(x, n)	( (x)& (~BIT(n)))
#define BSET(x, n)	( (x)| BIT(n))
#define BGET(x, n)	(((x)& BIT(n))>>(n))

#define LBYTE(x)	( (x)&0xFF)
#define HBYTE(x)	(((x)&0xFF00)>>8)

#define LWORD(x)	( (x)&0xFFFF)
#define HWORD(x)	(((x)&0xFFFF0000)>>16)	

#define MAKE_WORD(h, l)		((((h)&0xFF)<<8)   |((l)&0xFF))
#define MAKE_DWORD(h, l)	((((h)&0xFFFF)<<16)|((l)&0xFFFF))
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BITS_H_

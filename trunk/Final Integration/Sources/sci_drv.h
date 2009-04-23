/******************************************************************************		
*                                                                                   	
*       Copyright (C) 2006 Freescale Semiconductor, Inc.                            	
*       All Rights Reserved								              				
*														              					
* Filename:       SCI_DRV.H               											
*														              					
* Description:	  SCI Driver files for MC9S08QE128	        	    	
*                 based on Specification Rev.   	       						
*																						  	 		
******************************************************************************/		
/******************************************************************************/
#ifndef SCI_DRV_H        /*prevent duplicated includes*/
#define SCI_DRV_H

/************************* Macros ********************************************/

#include "derivative.H" /* include peripheral declarations */
#include "target.h"
#include "MC9S08QE128.h"

#define _CR PutChar(\r)
#define _LF PutChar(\n)
#define NEW_LINE DisplayString("\r\n")
#define LF NEW_LINE							


/************************* Prototypes ****************************************/

void SCI_Config(unsigned short);
void SCI_Off(void);
unsigned char convertASCII(unsigned char);
void DisplayByte(unsigned char); 
void DisplayString (unsigned char*);
void PutChar (unsigned char);
void DisplayLong(unsigned long, unsigned char); 
void DisplayInt(unsigned int, unsigned char);
unsigned char GetChar(void);


/*
extern char RecChar(void); //end RecChar  



extern void SendChar(char s_char);



/////////////////////////////////////////////////////////////////////////////////////////

// SendMsg

// --------------------------------------------------------------------------------------

// sends an ascii string out SCI1 at preset baudrate  

/////////////////////////////////////////////////////////////////////////////////////////

extern void SendMsg(char msg[]);





/////////////////////////////////////////////////////////////////////////////////////////

// byte2asc & word2asc

// --------------------------------------------------------------------------------------

// converts a byte or word into an ascii string of 2 or 4 chars

/////////////////////////////////////////////////////////////////////////////////////////

extern char * byte2asc(byte num, byte base);
extern char * word2asc(word num, byte base);

#define zero      0x30
#define one       0x31
#define two       0x32
#define three     0x33
#define four      0x34
#define five      0x35
#define six       0x36
#define seven     0x37
#define eight     0x38
#define nine      0x39
#define ten       0x61
#define enter     0x0D
#define space     0x20
#define EXIT      0x45
#define exit      0x65
#define yes       1
#define no        0
#define filter    2
#define avg       1
#define base10    1
#define base16    0


// RAM variables

static byte samp=0,MODE=0,n_str[5];
*/

#endif /* SCI_DRV_H */
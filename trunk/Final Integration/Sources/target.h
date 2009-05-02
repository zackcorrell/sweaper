/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** target.h
************************************************************************************     
*/		

#ifndef TARGET_H        /*prevent duplicated includes*/
#define TARGET_H


/*****************************************************************************
Stop Mode Entry from RAM
*****************************************************************************/
#if defined(__HCS08__)
#define Enter_Stop asm(jsr RamStop)
#else
#define Enter_Stop _Stop
#endif


/*****************************************************************************/

/******************************************************************************
User Defines 
******************************************************************************/

#define BUS_SPEED 20000000 /* Hz */
#define CONST (BUS_SPEED / 1000 /*ms*/ / 8 /* loop cycles*/ )

#define	OSCCLK_FREQ_KHZ		40000L	/* "L" postfix evaluated as a long value */
#define BUSCLK_FREQ_KHZ		20000L	
#define BUSCLK_FREQ_KHZ_MAX 20000L	/* "L" postfix evaluated as a long value */

	/* Bus frequency in kHz = fosc / 2 */
#define BUSCLK_FREQ_KHZ_OSC		OSCCLK_FREQ_KHZ/2


/******************************************************************************
Module Defines 
******************************************************************************/

/* SCI defines */ 
#define TERMINAL_BAUD_RATE	9600 
#define SPARE_BAUD_RATE 	9600

#define SCIBD_BAUD_RATE ((BUSCLK_FREQ_KHZ * 10) / (16 * (TERMINAL_BAUD_RATE/100)))
#define SCI_MODULO_DIVISOR ((BUS_SPEED / 100) / (16 * (TERMINAL_BAUD_RATE/100)))


/* ATD defines */
#define ATD_PRESCALAR 0x03


/* IIC defines */
#define MASTER 0xA0
#define SLAVE 0xF0
#define PACKETSIZE 8

/******************************************************************************
Function Prototypes 
******************************************************************************/

void Wait1ms(void);			   /* simple software loop delay */
void WaitNms(int);			   /* N x simple software loop delay */


/******************************************************************************
MACROS 
******************************************************************************/


/******************************************************************************
additional common definitions
******************************************************************************/

#define OFF     0
#define ON      1

#define CLEAR 	0
#define SET     1

#define FOREVER 1

#define FAIL  	0
#define PASS    1


#endif /* TARGET_H */

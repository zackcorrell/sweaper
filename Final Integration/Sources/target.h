/******************************************************************************		
*                                                                                   	
*       Copyright (C) 2006 Freescale Semiconductor, Inc.                            	
*       All Rights Reserved								              				
*														              					
* Filename:       TARGET.H               											
*														              					
* Description:	  Generic function definition files for MC9S08QE128	        	    	
*                 based on Specification Rev.   	       						
*																						
* Notes:        																		
*  ***********************************************************************  			
*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  	 		
*  *  FREESCALE CW COMPILER AND HAS ONLY BEEN GIVEN A MIMIMUM            *  	 		
*  *  LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES         *  	 		
*  *  AND NO PROMISE OF SUPPORT.                                         *  	 		
*  ***********************************************************************  	 		
*																						
* Freescale reserves the right to make changes without further notice to any			
* product herein to improve reliability, function, or design. Freescale does	 		
* not assume any  liability arising  out  of the  application or use of any 	 		
* product,  circuit, or software described herein;  neither  does it convey 	 		
* any license under its patent rights  nor the  rights of others.  Freescale	 		
* products are not designed, intended,  or authorized for use as components 	 		
* in  systems  intended  for  surgical  implant  into  the  body, or  other 	 		
* applications intended to support life, or  for any  other application  in 	 		
* which the failure of the Freescale product  could create a situation where	 		
* personal injury or death may occur. Should Buyer purchase or use Freescale	 		
* products for any such intended  or unauthorized  application, Buyer shall 	 		
* indemnify and  hold  Freescale  and its officers, employees, subsidiaries,	 		
* affiliates,  and distributors harmless against all claims costs, damages, 	  		
* and expenses, and reasonable  attorney  fees arising  out of, directly or 	 		
* indirectly,  any claim of personal injury  or death  associated with such 	 		
* unintended or unauthorized use, even if such claim alleges that  Freescale	 		
* was negligent regarding the  design  or manufacture of the part. Freescale	 		
* and the Freescale logo* are registered trademarks of Freescale Ltd.       	 		
******************************************************************************/		

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

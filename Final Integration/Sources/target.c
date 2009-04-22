/******************************************************************************		
*                                                                                   	
*       Copyright (C) 2006 Freescale Semiconductor, Inc.                            	
*       All Rights Reserved								              				
*														              					
* Filename:       TARGET.C               											
*														              					
* Description:	  Generic function definition files for MC9S08QE128	        	    	
*                 based on Specification Rev.   	       						       	 		
******************************************************************************/		
#include "target.h" /* include target header */
#include "MC9S08QE128.h"
/******************************************************************************
Function Name	:	Wait1ms
Engineer		:	
Date			:	02/06/00

Parameters		:	none
Returns			:	none
Notes			:	Waits 1mS. 
******************************************************************************/
 #include <hidef.h> /* for EnableInterrupts macro */

void Wait1ms(void){
 
 unsigned char i;
  
  for(i=0;i<255;i++) {}
       
}

/******************************************************************************
Function Name	:	WaitNms
Engineer		:	
Date			:	02/06/00

Parameters		:	int
Returns			:	none
Notes			:	Waits for N mS. 
******************************************************************************/

void WaitNms(int n){
   int i;
   for(i=1;i<=n;i++){
    Wait1ms();
   }
}



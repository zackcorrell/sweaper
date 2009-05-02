/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** ICS.c
************************************************************************************     
*/

#include "ICS.h"      


/******************************************************************************
Function Name	:	ICS_Config
Parameters		:	+CTL1      - initialization value for the ICS control register 1
                +CTL2      - initialization value for the ICS control register 2
                +TrimValue - Trim value for trimming the internal clock reference
                +CTL3      - initialization value for the ICSSC             
Returns			  :	NONE
******************************************************************************/
void ICS_Config(unsigned char CTL1, unsigned char CTL2,byte TrimValue, byte CTL3)
{


 
 if(CTL1 & IREFS_INTERNAL)
  {
   /* internal clock reference is */
  ICSC1  = CTL1;      /* init ISC control register 1 */
  ICSC2  = CTL2;      /* init ISC control register 2 */  
  ICSTRM = (unsigned char)TrimValue;           
  ICSSC  = CTL3;      /* init ISC control register 3 */
  
    if(ICSC1_CLKS == 0b01){
      /* FBI Mode wait for CLKST */
      while (ICSSC_CLKST != ICSC1_CLKS){}
    }
    else {
      /* FEI Mode wait for CLKST and DRST */
      while (ICSSC_CLKST != ICSC1_CLKS){}
      while ((ICSSC&ICSSC_DRST_DRS_MASK) != (CTL3&ICSSC_DRST_DRS_MASK) ){}   
    }
  }
 else
  {
    ICSC2           = CTL2;      /* init ISC control register 2 */  
    ICSC1           = CTL1;      /* init ISC control register 1 */
    ICSSC           = CTL3; 
    if(ICSC1_CLKS == 0b10){
      /* FBE Mode wait for CLKST */
      while (ICSSC_CLKST != ICSC1_CLKS){}
    }
    else {
      /* FEE Mode wait for CLKST and DRST */
      while (ICSSC_CLKST != ICSC1_CLKS){}
      while ((ICSSC&ICSSC_DRST_DRS_MASK) != (CTL3&ICSSC_DRST_DRS_MASK) ){}   
    }
    /* Wait for OSCINIT to set */
    while(!ICSSC_OSCINIT){}
  }
}
 
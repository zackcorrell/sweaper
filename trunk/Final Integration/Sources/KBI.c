#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "MC9S08QE128.h"

extern byte mode;

#define KBI_SW1 KBI1PE_KBIPE2_MASK | KBI1PE_KBIPE3_MASK
#define KBI_SW2 KBI2PE_KBIPE2_MASK | KBI2PE_KBIPE3_MASK

/////////////////////////////////////////////////////////////////////////////////////////
// KBI_INIT
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
void InitKBI(void) {
// Enable KBI1P[3:2] as interrupt
  KBI1PE = KBI_SW1;
  KBI2PE = KBI_SW2;
  KBI2SC = 0b00000110;
  KBI1SC = 0b00000110;
/*               ||||
                 |||+---- KBIMOD = KBI detection mode: 0=edge only
                 ||+----- KBIE   = KBI int enable: 1=enabled
                 |+------ KBACK  = KBI int acknowledge: 1=clr IRQF
                 +------- KBF    = KBI flag
*/
}

/////////////////////////////////////////////////////////////////////////////////////////
// KBI_ISR
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
/*
interrupt VectorNumber_Vkeyboard void KBI_ISR(void){
 
  //capture which pin was pushed

 
  mode = (byte)(((~PTAD)&0x0C)>>2);
  mode |= (byte)((~PTDD)&0x0C);


  //clear KBF
  KBI1SC_KBACK = 1;
  KBI2SC_KBACK = 1;
  
  
  
}
*/
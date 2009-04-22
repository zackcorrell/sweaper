/* Written Using CodeWarrior 6.0 ALPHA2 */

/******************************************************************************		
*                                                                                   	
*       Copyright (C) 2006 Freescale Semiconductor, Inc.                            	
*       All Rights Reserved								              				
*														              					
* Filename:       main.c               											
*														              					
* Description:	  main routine	        	    	
*																						     	 		
******************************************************************************/
/************************* Project Include Files *****************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "ICS.h"
#include "target.h"    
#include "sci_drv.h"
#include "MC9S08QE128.h"
#include "RunModes.h"
#include "ADC.h"
#include "SD_Card.h"
#include "Init.h"

/******************************************************************************
Board Specific Defines 
******************************************************************************/
#define BDM_ENABLE 1
#define EXTERNAL_CRYSTAL_AVAILABLE 1

const unsigned char FlsStop[0x02] = { 0x8E, 0x81};

volatile byte RTC_Flag=0; 
volatile byte Clk_Gate_Flag=0;

extern void InitKBI(void);

/*******************************************************************************
MAIN FUNCTION
*******************************************************************************/
void main(void) {
  
  unsigned char Need_Data=TRUE;
  //volatile byte Unitialized_RAM[15];  
  Sys_Peripheral_Init();
  ADC_Init();
  ADCSC1_AIEN = 1; // Enable ADC interrupt
  
    
  //test recovery from stop mode
  /*
  if(!SPMSC2_PPDF)             // if not recovering from stop: 
     POR_Boot();               // Print Initial Message                   
  else{
     PTCD    = Unitialized_RAM[0];         // restore led states
     Seconds = Unitialized_RAM[1];
     Minutes = Unitialized_RAM[2];
     Hours   = Unitialized_RAM[3];
     Stop2Recovery();          // if recoverying from stop2
  }
  */

  EnableInterrupts;
    
  for (;;){
    if(Clk_Gate_Flag){
                                        // Disable Clocks to unused peripherals
      SCGC1 = SCGC1_SCI1_MASK;  
      SCGC2 = SCGC2_RTC_MASK|SCGC2_KBI_MASK|SCGC2_IRQ_MASK;
      
    }else{
                                        // Enable Clocks to all unused peripherals
      SCGC1 = 0xFF;  
      SCGC2 = 0xFF;
    }

    if(Seconds==0x00||Seconds==0x0F||Seconds==0x1E||Seconds==0x2D){
       
       NormalRun();
       
    
       if(Need_Data){
       Read_Data();
              
       /*
       DisplayString("Solar Bytes (Upper then lower)");DisplayString("\r\n");
       DisplayByte(SolarU);DisplayString(":");DisplayByte(SolarL);DisplayString("\r\n");
       
       DisplayString("Temperature Bytes (Upper then lower)");DisplayString("\r\n");
       DisplayByte(TempU);DisplayString(":");DisplayByte(TempL);DisplayString("\r\n");                 
       
       DisplayString("....display complete.");DisplayString("\r\n");
       DisplayString("\r\n");
       }
       */
       
       Need_Data = FALSE;   

    }else{
    
       SleepRun();
       Need_Data=TRUE;
    
    }
    
    
  }   
 }   
}

/////////////////////////////////////////////////////////////////////////////////////////
// RTC Interrupt
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vrtc void _RTCISR(void)
{
    RTC_Flag = 0xFF;
    RTCSC_RTIF = 1;       // Clear Flag
    Seconds+=0x05;        // increment seconds
    if(Seconds>0x3B){
      Seconds = 0;
      Minutes++;
      if(Minutes>0x3B){
        Minutes = 0;
        Hours++;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// IRQ Interrupt
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Virq void _IRQISR( void )
{
    word i;
    Clk_Gate_Flag = (byte)(!Clk_Gate_Flag);
    if (mode!=2){
      for(i=0x1000;i>0;i--){}               // debounce IRQ pin (@20 MHz bus clock)
    } else {
      for(i=0x0008;i>0;i--){}               // debounce IRQ pin (@16 kHz bus clock)
    }
    
    IRQSC_IRQACK = 1;       // Clear Flag
}

/////////////////////////////////////////////////////////////////////////////////////////
// SCI Interrupt
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
/*
void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void) {
                              // SCI vector address = 15 (S08)
                              // SCI vector address = 77 (V1)
unsigned char temp;
SCI1S1_RDRF = 0;              // Receive interrupt disable
temp = SCI1D;                 // Store the recieve value on temp variable
while (SCI1S1_TDRE == 0);     // Wait for the transmitter to be empty
SCI1D = '1';                  // Send a character by SCI
}
*/

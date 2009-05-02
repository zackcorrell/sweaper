/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** RunModes.c
************************************************************************************     
*/

#include "RunModes.h"

void NormalRun(void){  //Full operation mode to take readings.
   RTC_Flag = 0;
   ICS_FEE_20MHz();                    // Configure ICS FEI 8MHz 
   WaitNms(100);                       // give FLL time to lock
   SPMSC2 = RUN_SPMSC2;                // Clear low power run flag
}

void SleepRun(void){   //Sleep mode for between sampling times.
   Enter_Stop;                         // MCU enters Stop 3 with BDM enabled/disabled and Vreg off
   WaitNms(100);                       // give FLL time to lock
   RTC_Flag = 0;
   ICS_FEE_20MHz();                    // Configure ICS FEI 8MHz
   WaitNms(100);                       // give FLL time to lock
   SPMSC2 = 0;                         // Clear low power run flag and enable stop3 mode
}
 


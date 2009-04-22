//////////////////////////////////////////////
//////    RunModes.c /////////////////////////
//////////////////////////////////////////////

#include "derivative.h" /* include peripheral declarations */
#include "ICS.h"
#include "target.h"    
#include "sci_drv.h"
#include "MC9S08QE128.h"
#include "RunModes.h"

void NormalRun(void){
   RTC_Flag = 0;
   ICS_FEE_20MHz();                    // Configure ICS FEI 8MHz 
   WaitNms(100);                       // give FLL time to lock
   SPMSC2 = RUN_SPMSC2;                // Clear low power run flag
}

void SleepRun(void){
   Enter_Stop;                         // MCU enters Stop 3 with BDM enabled/disabled and Vreg off
   WaitNms(100);                       // give FLL time to lock
   RTC_Flag = 0;
   ICS_FEE_20MHz();                    // Configure ICS FEI 8MHz
   WaitNms(100);                       // give FLL time to lock
   SPMSC2 = 0;                         // Clear low power run flag and enable stop3 mode
}
 


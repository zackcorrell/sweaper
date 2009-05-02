/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** ICS.h
************************************************************************************     
*/		
/******************************************************************************/
#ifndef ICS_H        /*prevent duplicated includes*/
#define ICS_H

#include "MC9S08QE128.h"
#include "derivative.h" /* include peripheral declarations */
/************************* Project Include Files *****************************/

/************************* Variables *****************************/


extern volatile byte RTC_Flag; 
extern volatile byte Clk_Gate_Flag;
static unsigned char RamStop[0x02]= { 0x8E, 0x81};

/****************************************************/
/*ICS configuration for the different operation mode*/
/****************************************************/
#define ICS_FEE_20MHz()     ICS_Config(0x00,0x07,0x80,0x60)         // Configure ICS FEE 20MHz
#define ICS_FBELP_16kHz()   ICS_Config(0x80,0x0F,0x80,0x20)         // Configure ICS FBE 32kHz

/***************************************************************************/
/* SPMSC2 Configurations for the different Operation Modes/Low Power modes */
/***************************************************************************/
#define INIT_SPMSC2       SPMSC2_PPDE_MASK
#define RUN_SPMSC2        0
#define LP_RUN_SPMSC2     SPMSC2_LPR_MASK
#define STOP3_SPMSC2      0
#define STOP2_SPMSC2      SPMSC2_PPDC_MASK
  

/************************* #Defines ******************************************/
#define CLKS_FLL           0x00
#define CLKS_INTERNAL      0x40
#define CLKS_EXTERNAL      0x80

#define RDIV_1             0x00
#define RDIV_2             0x08
#define RDIV_4             0x10
#define RDIV_8             0x18
#define RDIV_16            0x20
#define RDIV_32            0x28
#define RDIV_64            0x30
#define RDIV_128           0x38

#define IREFS_INTERNAL     0x04
#define IREFS_EXTERNAL     0x00

#define IRCLKEN_ACTIVE    0x02   /* enable internal clock reference to RTC */
#define IRCLKEN_INACTIVE  0x00

#define IREFSTEN_INTERNAL_STOP_ENABLE  0x01   
#define IREFSTEN_INTERNAL_STOP_DISABLE  0x00

#define BDIV_1            0x00
#define BDIV_2            0x40
#define BDIV_4            0x80
#define BDIV_8            0xC0

#define RANGE_HIGH        0x20
#define RANGE_LOW         0x00

#define HGO_HIGH_GAIN     0x10
#define HGO_LOW_PWR       0x00

#define LP_FLL_DISABLED   0x08
#define LP_FLL_ENABLED    0x00

#define EREFS_OSCILLATOR  0x04
#define EREFS_EX_CLOCK    0x00

#define ERCLKEN_ACTIVE    0x02   /* enable external clock refernce to RTC */
#define ERCLKEN_INACTIVE  0x00

#define EREFSTEN_INTERNAL_STOP_ENABLE  0x01   
#define EREFSTEN_INTERNAL_STOP_DISABLE  0x00

#define DCO_RANGE_LOW      0x00
#define DCO_RANGE_MID      0x40
#define DCO_RANGE_HIGH     0x80

#define DCO_MAX_25         0x00
#define DCO_MAX_TUNED      0x20

#define IREFST_EXTERNAL    0x10
#define IREFST_INTERNAL    0x00

#define CLKST_FLL           0x00
#define CLKST_INTERNAL      0x04
#define CLKST_EXTERNAL      0x08

#define OSCINIT_INIT        0x02
#define OSCINIT_NOINIT      0x00

/************************* Macros ********************************************/

/************************* Prototypes ****************************************/
void ICS_Config(unsigned char CTL1, unsigned char CTL2,byte TrimValue, unsigned char CTL3);


#endif /* ICS_H */
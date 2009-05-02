/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** ADC.c
************************************************************************************     
*/

#include "ADC.h"

volatile byte TempU,SolarUa, SolarUb=0x00;
volatile byte TempL,SolarLa,SolarLb=0x00;

void ADC_Init (void) {

ADCSC1 = 0x40; // Interrupt disable. One-conversion conversion and channel 0
               // active
ADCSC2 = 0x00; // Software trigger selected
ADCCFG = 0x34; // Input clock/2. Long Sample time configuration. 12-bit
               // conversion
APCTL1 = 0xF8; // ADC0/1/2 enabled
APCTL2 = 0xFF; // all other ADC input registers disabled
APCTL3 = 0xFF;
}

void Read_Data(void){

        ADCSC1_ADCH=0x00;            //Select AD input channel to read
        SolarUa=ADCRH;               //store upper and lower bytes respectively
        SolarLa=ADCRL;
        
        ADCSC1_ADCH=0x01;            //Select next input channel....up to 8 input channels
        SolarUb=ADCRH;               //can be read in this fashion, just initialize the 
        SolarLb=ADCRL;               //the array below accordingly.
     
        ADCSC1_ADCH=0x02;
        TempU=ADCRH;
        TempL=ADCRL; 
        
        ADCData[0]=SolarUa;          //Store all of new data into a single array
        ADCData[1]=SolarLa;
        ADCData[2]=SolarUb;
        ADCData[3]=SolarLb;
        ADCData[4]=TempU;
        ADCData[5]=TempL;       
    
}
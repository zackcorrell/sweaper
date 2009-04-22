#include "derivative.h"
#include "MC9S08QE128.h"
#include "ADC.h"
#include "sci_drv.h"

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

  //DisplayString("Reading...");DisplayString("\r\n");
  
        ADCSC1_ADCH=0x00;
        SolarUa=ADCRH;
        SolarLa=ADCRL;
        
        ADCSC1_ADCH=0x01;
        SolarUb=ADCRH;
        SolarLb=ADCRL;
     
        ADCSC1_ADCH=0x02;
        TempU=ADCRH;
        TempL=ADCRL; 
        
        ADCData[0]=SolarUa;
        ADCData[1]=SolarLa;
        ADCData[2]=SolarUb;
        ADCData[3]=SolarLb;
        ADCData[4]=TempU;
        ADCData[5]=TempL;       
    
  //DisplayString("....A2D complete.");DisplayString("\r\n");
}
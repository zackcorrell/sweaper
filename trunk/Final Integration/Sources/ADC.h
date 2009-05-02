/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** ADC.h
************************************************************************************     
*/

#ifndef ADC_H
#define ADC_H
               
#include "derivative.h"
#include "MC9S08QE128.h"               
               
void ADC_Init(void);
extern void Read_Data(void);

extern volatile unsigned char ADCData[6];                            
extern volatile byte TempU,TempL,SolarUa,SolarLa,SolarUb,SolarLb;


#endif /* ADC_H */
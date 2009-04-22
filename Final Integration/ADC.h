/////////////////////////////////////////////////////////////
//////////////                            ///////////////////
//////////////          ADC.h             ///////////////////
//////////////        21 Apr 09           ///////////////////
/////////////////////////////////////////////////////////////
#ifndef ADC_H
#define ADC_H

extern void ADC_Init(void);
extern void Read_Data(void);
//interrupt VectorNumber_Vadc ADC_ISR(void);

//extern volatile byte WindU,WindL;
extern volatile unsigned char ADCData[6];
extern volatile byte TempU,TempL,SolarUa,SolarLa,SolarUb,SolarLb;
//extern volatile byte HumidU,HumidL;

#endif /* ADC_H */
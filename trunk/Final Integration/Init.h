////////////////////////////////////////////////////
/////////////                             //////////
/////////////         Init.h              //////////
/////////////         16 Apr 09           //////////
////////////////////////////////////////////////////


#include "MC9S08QE128.h"
#include "derivative.h"
#include "SD_Card.h"


byte PortSense(byte Current);
void Sys_Peripheral_Init(void);
void SCI_Init (void);
//void Stop2Recovery(void);
void POR_Boot(void);
byte hex2bcd(byte hex);
void InitSystems(void);

void EnablePullups(void);

void GPIO_Init(void);
void RTC_Init (void);

void SCI_Init (void);
void SPI_Init (void);

void SDCard_Init(void);

extern volatile byte mode, Hours, Minutes, Seconds, State, backPTC;
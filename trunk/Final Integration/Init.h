////////////////////////////////////////////////////
/////////////                             //////////
/////////////         Init.h              //////////
/////////////         16 Apr 09           //////////
////////////////////////////////////////////////////


#include "MC9S08QE128.h"
#include "derivative.h"

byte PortSense(byte Current);
void Sys_Peripheral_Init(void);
void SCI_Init (void);
//void Stop2Recovery(void);
void POR_Boot(void);
byte hex2bcd(byte hex);

extern volatile byte mode, Hours, Minutes, Seconds, State, backPTC;
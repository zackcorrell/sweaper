/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** sci.h
************************************************************************************     
*/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/////////////////////////////////////////////////////////////////////////////////////////
// RecChar & SendChar
// --------------------------------------------------------------------------------------
// receives/sends an ascii char on SCI1 at preset baudrate  
/////////////////////////////////////////////////////////////////////////////////////////
extern char RecChar(void); //end RecChar  

extern void SendChar(char s_char);

/////////////////////////////////////////////////////////////////////////////////////////
// SendMsg
// --------------------------------------------------------------------------------------
// sends an ascii string out SCI1 at preset baudrate  
/////////////////////////////////////////////////////////////////////////////////////////
extern void SendMsg(char msg[]);


/////////////////////////////////////////////////////////////////////////////////////////
// byte2asc & word2asc
// --------------------------------------------------------------------------------------
// converts a byte or word into an ascii string of 2 or 4 chars
/////////////////////////////////////////////////////////////////////////////////////////
extern char * byte2asc(byte num, byte base);
extern char * word2asc(word num, byte base);

#define zero  0x30
#define one   0x31
#define two   0x32
#define three 0x33
#define four  0x34
#define five  0x35
#define six   0x36
#define seven 0x37
#define eight 0x38
#define nine  0x39
#define ten   0x61
#define enter 0x0D
#define space 0x20
#define EXIT  0x45
#define exit  0x65
#define yes   1
#define no    0
#define filter 2
#define avg    1
#define base10 1
#define base16 0

// RAM variables
static byte samp=0,MODE=0,n_str[5];



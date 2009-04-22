//////////////////////////////////////////////
//////////                      //////////////
//////////      Main.h          //////////////
//////////                      //////////////
//////////////////////////////////////////////
#include <hidef.h>
#include "MC9S08QE128.h"
#include "target.h"
#include "init.h"

/******************************************************************************
Board Specific Defines 
******************************************************************************/
#define BDM_ENABLE 1
#define EXTERNAL_CRYSTAL_AVAILABLE 1

const unsigned char FlsStop[0x02] = { 0x8E, 0x81};

volatile byte RTC_Flag=0; 
volatile byte Clk_Gate_Flag=0;

extern void InitKBI(void);

#define INIT 0;
#define SLEEP 1;
#define COLLECTDATA 2;
#define UPLOAD 3;
#define GETCURRENTDATA 4;

extern unsigned char state = INIT;

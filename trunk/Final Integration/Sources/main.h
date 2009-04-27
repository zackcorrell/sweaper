//////////////////////////////////////////////
//////////                      //////////////
//////////      Main.h          //////////////
//////////                      //////////////
//////////////////////////////////////////////
#include <hidef.h>
#include "MC9S08QE128.h"
#include "target.h"
#include "init.h"
#include "RunModes.h"
#include "ADC.h"
#include "sci.h"
#include "SD_Card.h"

/******************************************************************************
Board Specific Defines 
******************************************************************************/
#define BDM_ENABLE 1
#define EXTERNAL_CRYSTAL_AVAILABLE 1

const unsigned char FlsStop[0x02] = { 0x8E, 0x81};

volatile byte RTC_Flag=0; 
volatile byte Clk_Gate_Flag=0;

extern void InitKBI(void);

#define INIT 0
#define WAIT 1
#define SLEEP 2
#define COLLECTDATA 3
#define UPLOAD 4
#define GETCURRENTDATA 5
extern unsigned char state = WAIT;

unsigned char serialData = 0x00;
void initAll(void);

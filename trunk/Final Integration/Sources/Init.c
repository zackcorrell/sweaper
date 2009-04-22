//////////////////////////////////////////////////////
////////////                            //////////////
////////////        Init.c              //////////////
////////////        16 Apr 09           //////////////
//////////////////////////////////////////////////////

#include <hidef.h>
#include "MC9S08QE128.h"
#include "derivative.h"
#include "Init.h"
#include "ICS.h"
#include "target.h"

/////////////////////////////////////////////////////////////////////////////////////////
// POR Recovery
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
void POR_Boot(void){
     
     SPMSC2 = INIT_SPMSC2; // Enable control for stop2/stop3 modes
     mode = 1;
     State = 0;
     Seconds = 0;
     backPTC = 0; 
}



/////////////////////////////////////////////////////////////////////////////////////////
// Peripheral Init
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////

void Sys_Peripheral_Init(void){

  // enable STOP, BDM, RST and RSTO
  SOPT1 = SOPT1_STOPE_MASK|SOPT1_BKGDPE_MASK;  
  
  // Disable LVD
  SPMSC1 = 0x00;
  
  // Enable (but don't select) stop2 mode
  SPMSC2_PPDE = 1;       //PPDE is write once, so set during MCU initialization
  
  // for FEE  8MHz
  ICS_FEE_20MHz();      

  //ADC Init stuff
  SCGC1 = 0x10; // Bus Clock to the ADC module is enabled
  //SCGC2 = 0x00; // Disable Bus clock to unused peripherals 
      
  // Make all unused pins outputs
  PTADD = 0xF3;            
  PTBDD = 0xFF;           
  PTCDD = 0xFF;           
  PTDDD = 0xF3;           
  PTEDD = 0xFF;           
  PTFDD = 0xFF;           
  PTGDD = 0xFF;           
  PTHDD = 0xFF;           
  PTJDD = 0xFF; 
  
  PTED = 0x00;          

  //  Enable Pull Ups on all pins
  PTAPE = 0xFF;           
  PTBPE = 0xFF;
  PTCPE = 0xFF;
  PTDPE = 0xFF;
  PTEPE = 0xFF;
  PTFPE = 0xFF;
  PTGPE = 0xFF;
  PTHPE = 0xFF;
  PTJPE = 0xFF;
  
  // To turn on coms port on board
  PTGD_PTGD0 = 1;         
  
  //Turn off all LEDs
  PTCD = 0x3F;
  PTED = 0xC0;
  
  // Initialise RTC
  RTCSC = 0b00110110;
  /*        ||||||||
            ||||++++---Prescaler of 0110=2^15 (1 sec increment w/32768 xtal)
            |||+-------RTC interupts Enabled
            |++--------Ext osc selected as Clock Source
            +----------Flag  Bit */                      
  RTCMOD = 0x04;       // modulo of 4 sets RTC period to 5 secs
   
  //Initialize SCI
  SCI_Config(SCIBD_BAUD_RATE);

  //Initialize KBI
  InitKBI();

  //Initialize IRQ
  IRQSC = 0x14;
  IRQSC = 0x16;

}

/////////////////////////////////////////////////////////////////////////////////////////
// HEX to BCD
// --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
byte hex2bcd(byte hex){
byte dec[2],i;
byte bcd;

  for (i=0;i<2;i++){
    dec[i] = (byte)(hex%10);
    hex = (byte)(hex/10);
  }
  if (hex>0){
    bcd=0xff;
  }else{
    bcd=(byte)((dec[1]<<4) + dec[0]);
  }
  return bcd;
} //end hex2bcd

void SCI_Init (void) 
{
  SCI1C1 = 0x00; // 8-bit mode. Normal operation
  SCI1C2 = 0x2C; // Receiver interrupt enabled. Transmitter and receiver enabled
  SCI1C3 = 0x00; // Disable all errors interrupts
  SCI1BDL = 0x1A; // This register and the SCI1BDH are used to configure the SCI baud rate
  SCI1BDH = 0x00; // BUSCLK 4MHz
  // Baud rate = -------------------- = ------------ = 9600bps
} // [SBR12:SBR0] x 16 26 x 16
void InitSystems(void) 
{
  
  SOPT1  = 0x23;                          // Disable COP,RSTO, enable STOP,BKGD,RESET
  SOPT2  = 0x00;                          // SPI1 on PTB2/3/4/5, IIC1 on PTA2/3
  SPMSC1 = 0x00;                          // Disable LVD
  SPMSC2 = 0x00;                          // Disable power-down modes
  SPMSC3 = 0x00;                          // Disable LVWIE, low trip points
  SCGC1  = 0xFF;                          // Enable bus clock to peripherals
  SCGC2  = 0xFF;
  SCGC2 = 0x02;                          // Enable bus clock to peripherals
} // end InitSystems

void EnablePullups(void) 
{
  
  PTAPE = 0xFF;                           // Enable PORT A Internal Pullups
  PTBPE = 0xFF;                           // Enable PORT B Internal Pullups
  PTCPE = 0xFF;                           // Enable PORT C Internal Pullups
  PTDPE = 0xFF;                           // Enable PORT D Internal Pullups
  PTEPE = 0xFF;                           // Enable PORT E Internal Pullups
  PTFPE = 0xFF;                           // Enable PORT F Internal Pullups
  PTGPE = 0x0F;                           // Enable PTG0/1/2/3 Internal Pullups
  PTHPE = 0xC3;                           // Enable PTH0/1/6/7 Internal Pullups
} //end EnablePullups

void GPIO_Init(void) 
{
  
  //PTEDD |= 0xCB; // Configure PTE port as output
  //PTED |= 0xCB; // Put 0's in PTE port
  
  PTDDD |= 0x03;
  PTDD_PTDD0 = 1;
  PTDD_PTDD1 = 1; 
  
  PTCDD |= 0xFF; // Configure PTC port as output
  PTCD |= 0xFF; // Put 0's in PTC port
  PTADD |= 0x00; // Configure PTA port as input
  PTAPE = 0xFF;
  
}

void RTC_Init (void) 
{
  RTCSC = 0x01; // RTCPS configure prescaler period every 8ms
  RTCMOD = 0x00; // RTCMOD configure to interrupt every 8ms
}

void SPI_Init (void) 
{
  SPI2BR = 0x10;
  SPI2C1 = 0x50;
  SPI2C2 = 0x00; // Different pins for data input and data output
  
} 

void SDCard_Init()
{
  if(memCardInit())
	{
		if(setBLockLength())
		{
			sectorZero = getPartitionOffset();
		}
	}
  
}

void initAll(void)
{
 Sys_Peripheral_Init();
 ADC_Init();
 InitSystems();
  EnablePullups();
  GPIO_Init();
  RTC_Init ();
  SCI_Init();
  SPI_Init();
//  RTC_TICK = FALSE;
  SDCard_Init(); 
  
}
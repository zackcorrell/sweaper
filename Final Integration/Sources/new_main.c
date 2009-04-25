/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
************************************************************************************     
*/

#include "main.h"
#define INIT 0;
#define WAIT 1;
#define SLEEP 2;
#define COLLECTDATA 3;
#define UPLOAD 4;
#define GETCURRENTDATA 5;


//#pragma DATA_SEG __DIRECT_SEG DATA_BUFFER
//#pragma DATA_SEG DEFAULT 
/************************
   Main Program Loop
************************/
void main() 
{
int sectorNumber;
int bytesLeftInSector;
static unsigned char buffer[512];
unsigned char *bufferPtr;
unsigned char sleeping=0;

	initAll(); // set state flag to WAIT after initializing all modules etc.
	
	for(;;)
	{	
		switch(state)
		{	
		  case WAIT:
		       
		
		  case INIT:
			  sectorNumber = 1;
			  bytesLeftInSector = 512;
			  SendMsg("Case: INIT");SendChar(10);SendChar(13);
			  storeData(bufferPtr, sectorNumber, bytesLeftInSector,26);
			  state = COLLECTDATA;
			  break;
			
		  case SLEEP:
			  if(Seconds==0x00)
			  {
  				sleeping=0;
  				state = GETCURRENTDATA;
  				break;
  			}
  			else
  			{
  				if(sleeping==1)
  				{
  					break;
  				}
  				else
  				{
  					sleeping=1;
  					sleep();
  				}
  				break;
  			}
  		
  		case COLLECTDATA:
  			SendMsg("case: COLLECTDATA");SendChar(10);SendChar(13);
  			readSensors(); // readADC
  			storeData(bufferPtr, sectorNumber, bytesLeftInSector,6);
  			state = SLEEP;
  			break;
  			
  		case UPLOAD:
  			SendMsg("CASE: UPLOAD");SendChar(10);SendChar(13);
  			uploadData(bufferPtr,sectorNumber,bytesLeftInSector); // readData, sendData
  			state = INIT;
  			break;
  			
  		case GETCURRENTDATA:
  			SendMsg("CASE: GETCURRENTDATA");SendChar(10);SendChar(13);
  			sendCurrentData(); // sendData
  			state = SLEEP;
  			break;
  			
  		default:
  			SendMsg("before transmitData = real Data");SendChar(10);SendChar(13);
  			state = INIT;
  		}
	}
	
	
void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void) 
{
// SCI vector address = 15 (S08)
// SCI vector address = 77 (V1)
SendMsg("entered interrupt");SendChar(10);SendChar(13);
SCI1S1_RDRF = 0; // Receive interrupt disable
PTED = SCI1D; // Display on PTE the received data from SCI
while (SCI1S1_TDRE == 0); // Wait for the transmitter to be empty
SCI1D = '1'; // Send a character by SCI
}
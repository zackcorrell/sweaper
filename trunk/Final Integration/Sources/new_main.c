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
#define SLEEP 1;
#define COLLECTDATA 2;
#define UPLOAD 3;
#define GETCURRENTDATA 4;

extern unsigned char state = INIT;

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

	initAll(); // set state flag to INIT after initializing all modules etc.
	
	for(;;)
	{	
		switch(state)
		{	
		case INIT:
			sectorNumber = 1;
			bytesLeftInSector = 512;
			SendMsg("Case: INIT");SendChar(10);SendChar(13);
			storeData(bufferPtr, sectorNumber, bytesLeftInSector,numOfBytesToWrite);
			state = COLLECTDATA;
			break;
			
		case SLEEP:
			if(seconds==0x00)
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
			data = readSensors(); // readADC
			storeData(bufferPtr, sectorNumber, bytesLeftInSector,numOfBytesToWrite)
			state = SLEEP;
			break;
			
		case UPLOAD:
			SendMsg("CASE: UPLOAD");SendChar(10);SendChar(13);
			uploadData(); // readData, sendData
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
	
	
void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void) {
// SCI vector address = 15 (S08)
// SCI vector address = 77 (V1)
SCI1S1_RDRF = 0; // Receive interrupt disable
PTED = SCI1D; // Display on PTE the received data from SCI
while (SCI1S1_TDRE == 0); // Wait for the transmitter to be empty
SCI1D = '1'; // Send a character by SCI
}
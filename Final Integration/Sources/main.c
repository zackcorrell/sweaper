/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
** 
** Main.c
************************************************************************************     
*/

#include "main.h"

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
int index = 0;

	initAll(); // set state flag to WAIT after initializing all modules etc.
	for(;;)
	{	
		SendChar(0x33);
		switch(state)
		{	
		  case WAIT:
		     serialData = RecChar();
		     if(serialData =='U')
		      state = UPLOAD;
		     else if(serialData =='O')
		      state = INIT;
		     else if(serialData =='G')
		      state = GETCURRENTDATA;
		     break;
		     
		  case INIT:
        serialData = RecChar();
		    bufferPtr = buffer;
		    for(index=0;index<26;index++)
		    {
		      buffer[index] = RecChar();
		    }
			  sectorNumber = 1;
			  bytesLeftInSector = 512;
			  SendMsg("Case: INIT");SendChar(10);SendChar(13);
			  storeData(bufferPtr, sectorNumber, bytesLeftInSector,26);
			  for(index = 0;index<6;index++)
			  {
			    buffer[512-bytesLeftInSector++]=0x00;  
			  }
			  state = COLLECTDATA;
			  break;
			
		  case SLEEP:
			  serialData = RecChar();
			  if(serialData =='U')
			  {
			    NormalRun();
			    sleeping=0;
			    state = UPLOAD;
			    break;
			  }
			  else if(serialData =='G')
			  {
			    NormalRun();
			    sleeping=0;
			    state = GETCURRENTDATA;
			    break;
			  }
			  if(Seconds==0x00)
			  {
  				sleeping=0;
  				NormalRun();
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
  					SleepRun();
  				}
  				break;
  			}
  		
  		case COLLECTDATA:
  			SendMsg("case: COLLECTDATA");SendChar(10);SendChar(13);
  			Read_Data(); // readADC
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
}
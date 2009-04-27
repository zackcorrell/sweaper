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
	

/*
////////////////////////////////////////////////////////////////////////////////////
// SCI #1 TX Interrupt Service Routine
////////////////////////////////////////////////////////////////////////////////////
void interrupt VectorNumber_Vsci1tx SCI_TX_ISR(void)
{
	unsigned char temp;

	// Check if addtional data to be transmitted
	if(SCI.TxCnt != 0)
	{
		SCI.TxCnt--;
		temp = SCI1S1;
		SCI1D = *(SCI.TxBuf); //send out the next byte
		SCI.TxBuf++;
	}
	else
	{ 
    // Disable Tx
    SCI1C2_TE = DISABLE;      
		// Disable Tx ISR
		SCI1C2_TIE = DISABLE;
  }	
}


////////////////////////////////////////////////////////////////////////////////////
// SCI #1 RX Interrupt Service Routine
////////////////////////////////////////////////////////////////////////////////////
void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void)
{
  unsigned char rx_byte;

  if (SCI1S1_RDRF)    // 1st half of RDRF clear procedure
  {
    rx_byte = SCI1D;  // 2nd half of RDRF clear procedure
  }
  
  if (rx_byte == 'O') 
  {
                                        
  }else if (rx_byte == 'G')
  {
    
    //Read_Data();
    VectorNumber_Vsci1tx SCI_TX_ISR();
    //SCITx(unsigned char *ADCData[], unsigned char len);
  }else (rx_byte == 'U') 
  {
    readPartialSector(unsigned long sector,unsigned int byteOffset,unsigned int bytesToRead,unsigned char *buf); 
    SCITx(unsigned char *, unsigned char len);
  } 
}

/*
void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void)
{
  unsigned char rx_byte;

  if (SCI1S1_RDRF)    // 1st half of RDRF clear procedure
    rx_byte = SCI1D;  // 2nd half of RDRF clear procedure

  if ((rx_byte == START_BYTE)&&(!GOT_START_BYTE)) 
  {
                                    //GOT_START_BYTE = TRUE; 
                                    //PACKET_STARTED = FALSE;     
                                    //NEW_MSG_READY = FALSE;    
  } 
  else if ((rx_byte == START_BYTE)&&(GOT_START_BYTE))
  {
    GOT_START_BYTE = FALSE;
    PACKET_STARTED = TRUE;    
    NEW_MSG_READY = FALSE;    
  } 
  else if (PACKET_STARTED) 
  {
    GOT_START_BYTE = FALSE;
    PACKET_STARTED = FALSE;
    SCI.RxMsg = rx_byte;
    NEW_MSG_READY = TRUE; 
  } 
  else
  {
    GOT_START_BYTE = FALSE;
    PACKET_STARTED = FALSE;
  }
}

void interrupt VectorNumber_Vsci1tx SCI_TX_ISR(void)
{
	unsigned char temp;

	// Check if addtional data to be transmitted
	if(SCI.TxCnt != 0)
	{
		SCI.TxCnt--;
		temp = SCI1S1;
		SCI1D = *(SCI.TxBuf); //send out the next byte
		SCI.TxBuf++;
	}
	else
	{ 
    // Disable Tx
    SCI1C2_TE = DISABLE;      
		// Disable Tx ISR
		SCI1C2_TIE = DISABLE;
  }	
}


void interrupt VectorNumber_Vsci1rx SCI_RX_ISR(void) 
{
// SCI vector address = 15 (S08)
// SCI vector address = 77 (V1)
SendMsg("entered interrupt");SendChar(10);SendChar(13);
SCI1S1_RDRF = 0; // Receive interrupt disable
//PTED = SCI1D; // Display on PTE the received data from SCI
while (SCI1S1_TDRE == 0); // Wait for the transmitter to be empty
SCI1D = '1'; // Send a character by SCI
}

*/



////////////////////////////////////////
/////                          /////////
/////         SD_Card.c        /////////
/////         16 Apr 09        /////////
////////////////////////////////////////

#include "SD_Card.h"

static void delay(unsigned int d)
{
 unsigned int i;
 for(i = 0;i<d;i++)
 {
 }
}

/**
 *	checks if there is a memory card in the slot
 *	pulls 
 *
 *	@return	FALSE		card not detected
 *	@return TRUE		card detected
 **/
boolean detectCard(void)
{
	if(CD)
	{
	  return FALSE;
	}
	else
	{
	  return TRUE;
	}
}

/**
 *	sends a byte on the SPI to the memory card
 *
 *	@param	inBuf		byte to be sent
 **/
void spi_put(unsigned char inBuf)
{
  unsigned char junk = 0x39;
 // SendMsg("spi_put-> ");SendChar(10);SendChar(13);
  while(!SPI2S_SPTEF)// wait for transfer to finish
  {
    //SendMsg("waiting to send");SendChar(10);SendChar(13);
    
  }
  if(SPI2S_SPTEF==1)
  {
    // SendMsg("flag clear sending data to buffer");SendChar(10);SendChar(13);
  }
    
  SPI2D = inBuf;

}

/**
 *	reads a byte from the SPI to the memory card
 *
 *	@return	U1RXBUF		byte read
 **/

unsigned char spi_get(void)
{	
	
  unsigned char message = 0x39;
  //SendMsg("spi_get");SendChar(10);SendChar(13);
  
  while(!SPI2S_SPTEF);
  SPI2D = 0xFF;
  
  while(!SPI2S_SPRF)
  {
  
	  //SendMsg("receiving");SendChar(10);SendChar(13);
  }
	message = SPI2D;
	//SendMsg("spi_get return message->");SendChar(message);SendChar(10);SendChar(13);

	return message;

}

/**
 *	sends the command to the the memory card with the 4 arguments and the CRC
 *
 *	@param command	the command
 *	@param argX			the arguments
 *	@param CRC			cyclic redundancy check
 **/
void sendCommand(unsigned char command, unsigned char argA,
                 unsigned char argB, unsigned char argC,
                 unsigned char argD, unsigned char CRC)
{	
	spi_put(command);
	spi_put(argA);
	spi_put(argB);
	spi_put(argC);
	spi_put(argD);
	spi_put(CRC);
}

/**
 *	waits for the memory card to response
 *
 *	@param expRes		expected response
 *
 *	@return TRUE		card responded within time limit
 *	@return FALSE		card did not response
 **/
boolean cardResponse(unsigned char expRes)
{
	unsigned char actRes;
	unsigned int count = 256;
	actRes = 0xFF;
	while ((actRes != expRes) && (count > 0))
	{
		actRes = spi_get();
		//SendMsg("actRes=");SendChar(actRes);SendMsg(" expRes=");SendChar(expRes);SendChar(10);SendChar(13);		
		count--;
	}
	if (count == 0)
	{
	  //SendMsg("card did not respond in time");SendChar(10);SendChar(13);  
		return FALSE; 
	}
	return TRUE;
}

/**
 *	initializes the memory card by brign it out of idle state and 
 *	sets it up to use SPI mode
 *
 *	@return TRUE		card successfully initialized
 *	@return FALSE		card not initialized
 **/
boolean memCardInit(void)
{
	unsigned int count;
	CS =1;
	//MEM_PORT_OUT |=CS;	
	for (count = 0;count < 10; count++)
    {
		spi_put(0xFF);
    }
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD0,0,0,0,0,0x95);	// initializes the MMC
	//SendMsg("sent CMD0");SendChar(10);SendChar(13);	
	if(cardResponse(0x01))
	{				
		count = 255;
		do
		{
			sendCommand(CMD1,0,0,0,0,0xFF);	// brings card out of idle state
			count--;
		} while ((!cardResponse(0x00)) && (count > 0));
		CS = 1;
		//MEM_PORT_OUT |= CS;
		spi_put(0xFF);	
		if(count == 0)
			return FALSE;
	}
	else
	{
		CS = 1;
		//MEM_PORT_OUT |= CS;
		return FALSE;		
	}
	return TRUE;
}

/**
 *	Initialize the block length in the memory card to be 512 bytes
 *	
 *	return TRUE			block length sucessfully set
 *	return FALSE		block length not sucessfully set
 **/
boolean setBLockLength(void)
{
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD16,0,0,2,0,0xFF);
	if(cardResponse(0x00))
	{
		spi_put(0xFF);
		CS = 1;
		//MEM_PORT_OUT |= CS;
		return TRUE;
	}
	spi_put(0xFF);
	CS = 1;
	//MEM_PORT_OUT |= CS;
	return FALSE;
}

/**
 *	locates the offset of the partition table from the absolute zero sector
 *
 *	return ...			the offset of the partition table
 **/
unsigned long getPartitionOffset(void)
{
	unsigned int count = 0;
	unsigned long offset = 0;
	unsigned char charBuf;
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD17,0,0,0,0,0xFF);
	if(cardResponse(0x00))
	{
		if(cardResponse(0xFE))
		{
			for(count = 0; count < 454; count++)
			  spi_get();
			  charBuf = spi_get();
			  offset = charBuf;
			  charBuf = spi_get();
			  offset |= (unsigned long int)charBuf << 8;
			  charBuf = spi_get();
			  offset |= (unsigned long int)charBuf << 16;
			  charBuf = spi_get();
			  offset += (unsigned long int)charBuf << 24;
			  for(count = 458; count < SECTOR_SIZE; count++)
			  spi_get();
			  spi_get();
			  spi_get();
		}
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	return offset;
}

/**
 *	checks to see if previous write operation is sucessful
 *	
 *	@return TRUE		if data is successfully written to disk
 *	@return FALSE		if data is not successfully written to disk
 **/
boolean checkWriteState(void)
{
	unsigned int count = 0;
	char inRes;
	while(count <= 64)
	{
		inRes = spi_get();
		if((inRes & 0x1F) == 0x05)
			break;
		else if((inRes & 0x1F) == 0x0B)
			return FALSE;
		else if((inRes & 0x1F) == 0x0D)
			return FALSE;
		count++;
	}
	count = 0;
	while((count < 64) && (spi_get() == 0x00));
	if(count < 64)
		return TRUE;
	else
		return FALSE;
}

/**
 *	read content of the sector indicated by the input and place it in the buffer
 *
 *	@param	sector		sector to be read
 *	@param	*buf		pointer to the buffer
 *
 *	@return	...			number of bytes read
 **/
unsigned int readSector(unsigned long sector, unsigned char *buf)
{
  unsigned int count = 0;
	sector += sectorZero;
	sector *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD17,(sector>>16)&0xFF,(sector>>8)&0xFF,sector&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		if(cardResponse(0xFE))
		{
			for(count = 0; count < SECTOR_SIZE; count++)
			{
				buf[count] = spi_get();
			}
			spi_get();
			spi_get();
		}
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	return count;
}

/**
 *	read partial content of the sector indicated by the input and place it in the buffer
 *
 *	@param	sector		sector to be read
 *	@paran	*buf		pointer to the buffer
 *	@param	bytesOffset	starting point in the sector for reading
 *	@param	bytesToRead	number of bytes to read
 *
 *	@return	...			number of bytes read
 **/
unsigned int readPartialSector(unsigned long sector,
                               unsigned int byteOffset,
                               unsigned int bytesToRead,
                               unsigned char *buf)
{
  int count = 0;
	int countRead = 0;
	int leftover = SECTOR_SIZE - byteOffset - bytesToRead;
	sector += sectorZero;
	sector *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD17,(sector>>16)&0xFF,(sector>>8)&0xFF,sector&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		if(cardResponse(0xFE))
		{
			for(count = 0; count < byteOffset; count++)
				spi_get();
			for(countRead = 0; countRead < bytesToRead; countRead++)
			{
				buf[countRead] = spi_get();
			}
			for(count = 0; count < leftover; count++)
				spi_get();
			spi_put(0xFF);
			spi_put(0xFF);
		}
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	return countRead;
}

/**
 *	read partial content at the end of the sector1 indicated by the input and
 *	the begging of sector 2 and place it in the buffer
 *
 *	@param	sector1		first sector to be read
 *	@param	sector2		second sector to be read
 *	@paran	*buf		pointer to the buffer
 *	@param	bytesOffset	starting point in the sector for reading
 *	@param	bytesToRead	number of bytes to read
 *
 *	@return	...			number of bytes read
 **/
unsigned int readPartialMultiSector(unsigned long sector1,
                                    unsigned long sector2,
                                    unsigned int byteOffset,
                                    unsigned int bytesToRead,
                                    unsigned char *buf)
{
	int count = 0;
	int countRead = 0;
	sector1 += sectorZero;
	sector1 *= 2;	
	sector2 += sectorZero;
	sector2 *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD17,(sector1>>16)&0xFF,(sector1>>8)&0xFF,sector1&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		if(cardResponse(0xFE))
		{
			for(count = 0; count < byteOffset; count++)
				spi_get();
			for(count = byteOffset; count < SECTOR_SIZE; count++, countRead++)
			{
				buf[countRead] = spi_get();
			}
			spi_put(0xFF);
			spi_put(0xFF);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	spi_get();
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD17,(sector2>>16)&0xFF,(sector2>>8)&0xFF,sector2&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		if(cardResponse(0xFE))
		{			
			for(count = 0; countRead < bytesToRead; count++, countRead++)
			{
				buf[countRead] = spi_get();
			}
			for(count = count; count < SECTOR_SIZE; count++)
				spi_get();
			spi_put(0xFF);
			spi_put(0xFF);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
    return countRead;
}

/**
 *	writes content of the buffer to the sector indicated by the input
 *
 *	@param	sector		sector to be written
 *	@param	*buf		pointer to the buffer
 *
 *	@return	...			number of bytes written
 **/
unsigned int writeSector(unsigned long sector, unsigned char *buf)
{
  unsigned int count = 0;
	sector += sectorZero;
	sector *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;

	sendCommand(CMD24,(sector>>16)&0xFF,(sector>>8)&0xFF,sector&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		spi_put(0xFF);
		spi_put(0xFE);
		for(count = 0; count < SECTOR_SIZE; count++)
		{
			spi_put(buf[count]);
		}
		spi_get();
		spi_get();
		checkWriteState();
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	return count;
}

/**
 *	write the content of the buffer indicated by the input to the MMC/SD card location
 *	indicated by the input sector.
 *
 *	@param	sector		sector to be written to
 *	@paran	*buf		pointer to the buffer
 *	@param	bytesOffset	starting point in the sector for writing
 *	@param	bytesToRead	number of bytes to write
 *
 *	@return	...			number of bytes written
 **/
unsigned int writePartialSector(unsigned long sector,
                                unsigned int byteOffset,
                                unsigned int bytesToWrite,
                                unsigned char *buf)
{
  int count = 0;
	int countWrote = 0;	
	unsigned char readBuf[SECTOR_SIZE];
	readSector(sector,readBuf);
	sector += sectorZero;
	sector *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD24,(sector>>16)&0xFF,(sector>>8)&0xFF,sector&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		spi_put(0xFF);
		spi_put(0xFE);
		for(count = 0; count < byteOffset; count++)		
			spi_put(readBuf[count]);
		for(countWrote = 0; countWrote < bytesToWrite; countWrote++, count++)
		{
			spi_put(buf[countWrote]);
		}
		for(count = count; count < SECTOR_SIZE; count++)
			spi_put(readBuf[count]);
		spi_get();
		spi_get();
		checkWriteState();
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	return countWrote;
}

/**
 *	write the content of the buffer to the end of the sector1
 *	and the begging of sector 2
 *
 *	@param	sector1			first sector to be written to
 *	@param	sector2			second sector to be written to
 *	@paran	*buf			pointer to the buffer
 *	@param	bytesOffset		starting point in the sector for reading
 *	@param	bytesToWrite	number of bytes to write
 *
 *	@return	...				number of bytes written
 **/
unsigned int writePartialMultiSector(unsigned long sector1,
                                     unsigned long sector2,
                                     unsigned int byteOffset,
                                     unsigned int bytesToWrite,
                                     unsigned char *buf)
{
  unsigned int count = 0;
	unsigned int countWrote = 0;	
	unsigned int writeSector2 = bytesToWrite - SECTOR_SIZE + byteOffset;
	unsigned char readBuf[SECTOR_SIZE];
	readSector(sector1,readBuf);
	sector1 += sectorZero;
	sector1 *= 2;	
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD24,(sector1>>16)&0xFF,(sector1>>8)&0xFF,sector1&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		spi_put(0xFF);
		spi_put(0xFE);
		for(count = 0; count < byteOffset; count++)		
			spi_put(readBuf[count]);
		for(count = byteOffset; count < SECTOR_SIZE; countWrote++, count++)
		{
			spi_put(buf[countWrote]);
		}
		spi_get();
		spi_get();
		checkWriteState();
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	readSector(sector2,readBuf);
	sector2 += sectorZero;
	sector2 *= 2;
	CS = 0;
	//MEM_PORT_OUT &= ~CS;
	sendCommand(CMD24,(sector2>>16)&0xFF,(sector2>>8)&0xFF,sector2&0xFF,0,0xFF);
	if(cardResponse(0x00))
	{
		spi_put(0xFF);
		spi_put(0xFE);
		for(count = 0; count < writeSector2; countWrote++, count++)
		{
			spi_put(buf[countWrote]);
		}
		for(count = writeSector2; count < SECTOR_SIZE; count++)
			spi_put(readBuf[count]);		
		spi_get();
		spi_get();
		checkWriteState();
	}
	CS = 1;
	//MEM_PORT_OUT |= CS;
	spi_get();
	spi_get();
	return countWrote;
}
 void storeData(unsigned char *bufferPtr, int sectorNumber,int bytesLeftInSector,int numOfBytesToWrite)
{
 
 int i = 0;
 unsigned char dataBuffer[512];
 if(numOfBytesToWrite==0)
    return;
 
 if(bytesLeftInSector<=0)
 {
    SDCard_Init();
    writeSector(sectorNumber,bufferPtr);
    sectorNumber = sectorNumber+1;
    bytesLeftInSector = 512;
 } 
 for(i=0;i<numOfBytesToWrite;i++)
 {
   dataBuffer[512-bytesLeftInSector++] = ADCData[i];
 }
}

void uploadData(unsigned char *bufferPtr,int sectorNumber,int bytesLeftInSector)
{
  int h = 0;
  int i = 0;
  int j = 0;
  int numOfBytesToWrite = 512-bytesLeftInSector;
  unsigned char readBuffer[512];
  unsigned char *readBufferPtr;
  
  readBufferPtr = readBuffer;
  // fill sector with zeros
  for(h = 512-bytesLeftInSector;h<512;h++)
  {
    dataBuffer[h]=0; 
  }
  storeData(bufferPtr, sectorNumber, bytesLeftInSector,numOfBytesToWrite);
  for(i = 1;i<=sectorNumber;i++)
  {
    SDCard_Init();
    readSector(i,readBufferPtr);
    for(j = 0;j<512;j++)
    {
      putChar(readBuffer[i]);
    }
  }
}

void sendCurrentData()
{
 int i = 0;
 readSensors();
 for(i = 0;i<6;i++)
    {
      putChar(ADCData[i]);
    }  
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
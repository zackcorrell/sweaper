//////////////////////////////////////////////////
//////////                          //////////////
//////////        SD_Card.h         //////////////
//////////        16 Apr 09         //////////////
//////////////////////////////////////////////////

#include <hidef.h>
#include "MC9S08QE128.h"
#include "derivative.h"
#include "ADC.h"


/* Memory card sector size */
#define SECTOR_SIZE 512
extern unsigned long sectorZero;
extern unsigned char dataBuffer[512];
/**
 * MMC/SD card SPI mode commands
 **/
#define CMD0 0x40	// software reset
#define CMD1 0x41	// brings card out of idle state
#define CMD2 0x42	// not used in SPI mode
#define CMD3 0x43	// not used in SPI mode
#define CMD4 0x44	// not used in SPI mode
#define CMD5 0x45	// Reserved
#define CMD6 0x46	// Reserved
#define CMD7 0x47	// not used in SPI mode
#define CMD8 0x48	// Reserved
#define CMD9 0x49	// ask card to send card specific data (CSD)
#define CMD10 0x4A	// ask card to send card identification (CID)
#define CMD11 0x4B	// not used in SPI mode
#define CMD12 0x4C	// stop transmission on multiple block read
#define CMD13 0x4D	// ask the card to send it's status register
#define CMD14 0x4E	// Reserved
#define CMD15 0x4F	// not used in SPI mode
#define CMD16 0x50	// sets the block length used by the memory card
#define CMD17 0x51	// read single block
#define CMD18 0x52	// read multiple block
#define CMD19 0x53	// Reserved
#define CMD20 0x54	// not used in SPI mode
#define CMD21 0x55	// Reserved
#define CMD22 0x56	// Reserved
#define CMD23 0x57	// Reserved
#define CMD24 0x58	// writes a single block
#define CMD25 0x59	// writes multiple blocks
#define CMD26 0x5A	// not used in SPI mode
#define CMD27 0x5B	// change the bits in CSD
#define CMD28 0x5C	// sets the write protection bit
#define CMD29 0x5D	// clears the write protection bit
#define CMD30 0x5E	// checks the write protection bit
#define CMD31 0x5F	// Reserved
#define CMD32 0x60	// Sets the address of the first sector of the erase group
#define CMD33 0x61	// Sets the address of the last sector of the erase group
#define CMD34 0x62	// removes a sector from the selected group
#define CMD35 0x63	// Sets the address of the first group
#define CMD36 0x64	// Sets the address of the last erase group
#define CMD37 0x65	// removes a group from the selected section
#define CMD38 0x66	// erase all selected groups
#define CMD39 0x67	// not used in SPI mode
#define CMD40 0x68	// not used in SPI mode
#define CMD41 0x69	// Reserved
#define CMD42 0x6A	// locks a block
                    // CMD43 ... CMD57 are Reserved
#define CMD58 0x7A	// reads the OCR register
#define CMD59 0x7B	// turns CRC off
                    // CMD60 ... CMD63 are not used in SPI mode




#define CD  PTAD_PTAD0
#define CS  PTCD_PTCD0  //board pin 13

#define to_boolean(b)   (((b) != 0) ? TRUE : FALSE)
typedef unsigned char   boolean;                                            boolean detectCard(void);


void spi_put(unsigned char inBuf);
unsigned char spi_get(void);
void sendCommand(unsigned char command, unsigned char argA,
                 unsigned char argB, unsigned char argC,
                 unsigned char argD, unsigned char CRC);
boolean cardResponse(unsigned char expRes);
boolean memCardInit(void);  
boolean setBLockLength(void);
unsigned long getPartitionOffset(void);
boolean checkWriteState(void);
unsigned int readSector(unsigned long sector, unsigned char *buf);
unsigned int readPartialSector(unsigned long sector,
                               unsigned int byteOffset,
                               unsigned int bytesToRead,
                               unsigned char *buf);
unsigned int readPartialMultiSector(unsigned long sector1,
                                    unsigned long sector2,
                                    unsigned int byteOffset,
                                    unsigned int bytesToRead,
                                    unsigned char *buf);
unsigned int writeSector(unsigned long sector, unsigned char *buf);
static void delay(unsigned int d);
volatile byte Save_Data(volatile byte A,
                        volatile byte B, 
                        volatile byte C, 
                        volatile byte D, 
                        volatile byte E, 
                        volatile byte F);
                        
void storeData(unsigned char *bufferPtr, int sectorNumber,int bytesLeftInSector,int numOfBytesToWrite);
void uploadData(unsigned char *bufferPtr,int sectorNumber,int bytesLeftInSector);
void sendCurrentData(void);
void SDCard_Init(void);

                            
                    
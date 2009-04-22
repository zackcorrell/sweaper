/******************************************************************************		
*                                                                                   	
*       Copyright (C) 2006 Freescale Semiconductor, Inc.                            	
*       All Rights Reserved								              				
*														              					
* Filename:       SCI_DRV.C               											
*														              					
* Description:	  SCI Driver files for MC9S08QE128	        	    	
*                 based on Specification Rev.   	       						
*																						
******************************************************************************/		

/* include files */

#include "derivative.H" /* include peripheral declarations */
#include "target.h"
#include "sci_drv.h"
#include "MC9S08QE128.h"

/******************************************************************************
Function Name	:	ICS_Config
Engineer      :	r57118
Date          :	22/06/06

Parameters		:	+CTL1      - initialization value for the ICS control register 1
                +CTL2      - initialization value for the ICS control register 2
                +TrimValue - Trim value for trimming the internal clock reference
                             This parameter is only used when internal clock 
                             reference is selceted 
Returns			  :	NONE
Notes         :	Configures ICS
******************************************************************************/

/******************************************************************************
Function Name	:	SCI_Config
Engineer		  :	ttz890
Date			    :	03/01/07

Parameters	  :	NONE
Returns			  :	NONE
Notes			    :	Configures SCI
******************************************************************************/
void SCI_Config(unsigned short prescaler)
{
  
  SCGC1 = SCGC1_SCI1_MASK;  
  WaitNms(10);                     // Wait 10ms
		/* set baud rate */
	SCI1BD = prescaler;
		/* set control register 1 */
	SCI1C1 = 0x00;
		/* enable transmit */
	SCI1C2_TE = 1;
		/* enable receive */
	SCI1C2_RE = 1;
}

/******************************************************************************
Function Name	:	SCI_Config
Engineer		  :	ttz890
Date			    :	03/01/07

Parameters	  :	NONE
Returns			  :	NONE
Notes			    :	Configures SCI
******************************************************************************/
void
SCI_Off(void)
{

		/* enable transmit */
	SCI1C2_TE = 0;
		/* enable receive */
	SCI1C2_RE = 0;
  SCGC1 &= (~SCGC1_SCI1_MASK);
}

/******************************************************************************
Function Name	:	DisplayString
Engineer		  :	r58711
Date			    :	14/02/02

Parameters		:	textString[]
Returns			  :	NONE
Notes			    :	Output text string as ASCII to SCI
******************************************************************************/
void DisplayString (unsigned char *textPointer)
{
		/* while not end of string */
	while(*textPointer != 0)
	{	
			/* write the character to the SCI interface */
		PutChar(*textPointer);
			/* increment to point at the next character in the string */
		textPointer++;
	}
	while(!SCI1S1_TC){}
}

/******************************************************************************
Function Name	:	DisplayByte
Engineer		  :	r58711
Date			    :	14/02/02

Parameters		:	displayByte
Returns			  :	NONE
Notes			    :	Output variable SCI after conversion to ASCII
******************************************************************************/
void DisplayByte (unsigned char displayByte)
{
	unsigned char upperNibble, lowerNibble;
		/* Split data into nibbles for ASCII conversion */
	upperNibble = (unsigned char)((displayByte & 0xF0)>>4); 
	lowerNibble = (unsigned char)(displayByte & 0x0F);
		/* Pass nibbles to SCI after passing for conversion to ASCII */
	PutChar(convertASCII(upperNibble));
	PutChar(convertASCII(lowerNibble));
	while(!SCI1S1_TC){}
}

/******************************************************************************
Function Name	  :	PutChar
Engineer		    :	r32151
Date			      :	22/01/02

Parameters		  :	char to be output to SCI1
Returns			    :	NONE
Notes			      :	
******************************************************************************/
void PutChar(unsigned char ch) 
{
	byte dummy;
		/* check SCI transmit data register is empty */
	dummy = SCI1S1;
	while(SCI1S1_TDRE == 0)
	{
	}	
	SCI1D = ch;	
}

/******************************************************************************
Function Name	:	GetChar
Engineer		  :	r32151
Date			    :	04/11/02

Parameters		:	char to be output to SCI
Returns			  :	NONE
Notes			    :	
******************************************************************************/
unsigned char GetChar(void) 
{
	byte dummy;
		/* check SCI1 receive data register is empty */
	dummy = SCI1S1;
	while(SCI1S1_RDRF == 0)
	{
	}	
	return(SCI1D);	
}


/******************************************************************************
Function Name	:	convertASCII
Engineer		  :	r58711
Date			    :	15.11.01

Parameters		:	Nibble of binary data (0000 to 1111) (unsigned char)
Returns			  :	ASCII code for nibble (unsigned char)
*******************************************************************************/
unsigned char convertASCII(unsigned char Nibble)
{	
	/* If nibble is less than 9 (ie. a number) */
	if (Nibble <= 0x09) 
		/* Return the ASCII code for this number */
		return ((unsigned char)(Nibble + 0x30)); 
	else
		/* Return the ASCII code for this letter */
		return ((unsigned char)(Nibble + 0x37)); 
} 

/******************************************************************************
Function Name	:	DisplayInt
Engineer		  :	r32151
Date			    :	04/09/02

Parameters		:	int count value to display tU08 field width
Returns			  :	NONE
Notes			    :	Effectively perforns a number to string conversion and then
					      outputs the string.
					      if field width = 0 then the int is displayed left justified   
					      with leading zeros supressed.
					      if field width = 1-5 then lowest n digits are displayed
					      with leading zeros.
					      This routine does not check that the field size is valid.					
******************************************************************************/
void 
DisplayInt(unsigned int number, unsigned char field ) 
{
	unsigned char buf[6];
	char i = 5;

	buf[i] = 0;
			/* for upto 10 digits : max value for long = 4294967296 */
			/* using a do while loop to take care of the number = 0 */
	do
	{
				/* convert least significant character of the int into ascii -> string buffer */
		buf[--i] = (unsigned char)((number % 10) + '0');
				/* moves next digit to least significant position */
		number /= 10;
	} while (number != 0); 
				/* if field width is less than number of digits only o/p field width of string */
	if ((field < (5-i)) && (field != 0))
		i = (char)(5 - field);
				/* if field width is more than number of digits add leading zeros */
	if (field > (5-i))
	{
		while ((5-i) < field)
	    {
				/* add leading zeros */ 
		buf[--i] = '0';
		}
	}
	DisplayString(&buf[i]);	
}
	
/******************************************************************************
Function Name	:	DisplayLong
Engineer		  :	r32151
Date			    :	04/09/02

Parameters		:	long count value to display tU08 field width
Returns			  :	NONE
Notes			    :	Direct replacement for NumberDisplay().

				    	  Effectively perforns a number to string conversion and then
					      outputs the string.
					      if field width = 0 then the int is displayed left justified   
					      with leading zeros supressed.
					      if field width = 1-10 then lowest n digits are displayed
					      with leading zeros.
					      This routine does not check that the field size is valid.
					
					      Due to the 32 bit math routines for handling long values
					      code size and execution time is significantly larger that
					      for DisplayInt.
******************************************************************************/
void 
DisplayLong(unsigned long number, unsigned char field ) 
{
	unsigned char buf[11];
	char i = 10;
	char length = 0;

	buf[i] = 0;
			/* for upto 10 digits : max value for long = 4294967296 */
			/* using a do while loop to take care of the number = 0 */
	do
	{
				/* convert least significant character of the int into ascii -> string buffer */
		buf[--i] = (unsigned char)(number % 10 + '0');
				/* moves next digit to least significant position */
		number /= 10;
	} while (number != 0); 
				/* if field width is less than number of digits only o/p field width of string */
	if ((field < (10-i)) && (field != 0))
		i = (char)(10 - field);
				/* if field width is more than number of digits add leading zeros */
	if (field > (10-i))
	{
		while ((10-i) < field)
	    {
				/* add leading zeros */ 
		buf[--i] = '0';
		}
	}
	DisplayString(&buf[i] );	
}	

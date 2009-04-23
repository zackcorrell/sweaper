#include "SCI.h" /* include main program defines and declarations */


/////////////////////////////////////////////////////////////////////////////////////////
// RecChar & SendChar
// --------------------------------------------------------------------------------------
// receives/sends an ascii char on SCI1 at preset baudrate  
/////////////////////////////////////////////////////////////////////////////////////////
char RecChar() {
  byte rec_char;

  if (SCI1S1_RDRF)  // 1st half of RDRF clear procedure
    rec_char = SCI1D;  // 2nd half of RDRF clear procedure
  SCI1C2_RE = 1;    // enable Rx
  while(!SCI1S1_RDRF){  };
  rec_char = SCI1D; // get recieved character
  SendChar((char) rec_char); // echo received character
  return (char) SCI1D;
} //end RecChar  

void SendChar(char s_char) {
  
  SCI1C2 = 0x08;    // enable Tx
  while(!SCI1S1_TDRE){ }
  SCI1D = (byte) s_char;   // 2nd half of TDRE clear procedure
} //end SendChar

/////////////////////////////////////////////////////////////////////////////////////////
// SendMsg
// --------------------------------------------------------------------------------------
// sends an ascii string out SCI1 at preset baudrate  
/////////////////////////////////////////////////////////////////////////////////////////
void SendMsg(char msg[]) {
  byte i=0;
  char nxt_char;
  
  SCI1C2 = 0x08;    // enable Tx
  nxt_char = msg[i++];
  while(nxt_char != 0x00) {
    while(!SCI1S1_TDRE){}
    SCI1D = (byte) nxt_char; // 2nd half of TDRE clear procedure
    nxt_char = msg[i++];
  } //end while((SCI1D
} //end SendMsg

/////////////////////////////////////////////////////////////////////////////////////////
// byte2asc & word2asc
// --------------------------------------------------------------------------------------
// converts a byte or word into an ascii string of 2 or 4 chars
/////////////////////////////////////////////////////////////////////////////////////////
char * byte2asc(byte num, byte base) {
byte n;

  if (base){
    n=(byte)(hex2bcd(num));
  }else{
    n=num;
  } //end if (base)
  n_str[0] = (byte)((n>>0x04)+0x30);  // convert MSN to ascii
  if(n_str[0]>0x39)           // if MSN is $a or larger...
    n_str[0]+=0x07;           // ...add $7 to correct
  n_str[1] = (byte)((n&0x0f)+0x30);   // convert LSN to ascii
  if(n_str[1]>0x39)           // if LSN is $a or larger...
    n_str[1]+=0x07;           // ...add $7 to correct
  n_str[2] = 0x00;            // add line feed
  return  (char *) n_str;
} //end byte2asc

char * word2asc(word num, byte base) {
word n;

  if (base){
    n=hex2bcd(num);
  }else{
    n=num;
  } //end if (base)

  n_str[0] = (byte)((n>>12)+0x30);    // convert MSN to ascii
  if(n_str[0]>0x39)           // if MSN is $a or larger...
    n_str[0]+=0x07;           // ...add $7 to correct
  n_str[1] = (byte)(((n>>8)&0x0f)+0x30);   // convert 2nd MSN to ascii
  if(n_str[1]>0x39)           // if LSN is $a or larger...
    n_str[1]+=0x07;           // ...add $7 to correct
  n_str[2] = (byte)(((n>>4)&0x0f)+0x30);   // convert 2nd MSN to ascii
  if(n_str[2]>0x39)           // if LSN is $a or larger...
    n_str[2]+=0x07;           // ...add $7 to correct
  n_str[3] = (byte)((n&0x0f)+0x30);   // convert 2nd MSN to ascii
  if(n_str[3]>0x39)           // if LSN is $a or larger...
    n_str[3]+=0x07;           // ...add $7 to correct
  n_str[4] = 0x00;    // add line feed
  return  (char *) n_str;
    
} //end word2asc
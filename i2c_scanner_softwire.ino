// --------------------------------------
// i2c_scanner
//
//

#include <SoftWire.h>

SoftWire SWire(PF1, PF2, SOFT_FAST);
  const byte addr = 0x5E; // default address of magnetic sensor 0x5E or 0x3E
  byte rbuffer[10];       // store data from sensor read registers
  byte wbuffer[4];
  int num=0;
  const byte lpm1[]  = { B00000000, B00000101, B00000000, B11000000 }; // low power mode
  const byte lpm2[]  = { B00000000, B00100101, B00000000, B11000000 }; // low power mode
  const byte lpm3[]  = { B00000000, B01000101, B00000000, B11000000 };
  const byte lpm4[]  = { B00000000, B01100101, B00000000, B11000000 };


int decodeX(int a, int b){
/* Shift all bits of register 0 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0:3 shift in as zero.
 * Determine which of bits 4:7 of register 4 are high, shift them to the right four places -- remask in case
 * they shift in as something other than 0.  bitRead and bitWrite would be a bit more elegant in next version
 * of code.
 */
  int ans = ( a << 4 ) | (((b & B11110000) >> 4) & B00001111);
  if( ans > 2047){ ans -= 4096; } // Interpret bit 12 as +/-
  return ans;
  }

int decodeY(int a, int b){
/* Shift all bits of register 1 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0-3 shift in as zero.
 * Determine which of the first four bits of register 4 are true.  Add to previous answer.
 */

  int ans = (a << 4) | (b & B00001111);
  if( ans > 2047){ ans -= 4096;} // Interpret bit 12 as +/-
  return ans;
}

int decodeZ(int a, int b){
/* Shift all bits of register 2 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0-3 are zero.
 * Determine which of the first four bits of register 5 are true.  Add to previous answer.
 */
  int ans = (a << 4) | (b & B00001111);
  if( ans > 2047){ ans -= 4096;}
  return ans;
}

int decodeT(int a, int b){
/* Determine which of the last 4 bits of register 3 are true.  Shift all bits of register 3 to the left 
 * 4 positions.  Bit 8 becomes bit 12.  Bits 0-3 are zero.
 * Determine which of the first four bits of register 6 are true.  Add to previous answer.
 */
  int ans;
  a &= B11110000;
  ans = (a << 4) | b;
  if( ans > 1024){ ans -= 4096;}
  return ans;
}

void fun(void)
{float x = decodeX(rbuffer[0],rbuffer[4]);
float y = decodeY(rbuffer[1],rbuffer[4]);
float z = decodeZ(rbuffer[2],rbuffer[5]);
Serial.print("Bx:");
Serial.print(x*0.098,3); Serial.print("mT");
Serial.print("\t");
Serial.print("By:");
Serial.print(y*0.098,3);Serial.print("mT");
Serial.print("\t");
Serial.print("Bz:");
Serial.print(z*0.098,3);Serial.print("mT");
Serial.print("\t");
}




void setup() {
  pinMode(PB11, OUTPUT);
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PE5, OUTPUT);
  digitalWrite(PB11, HIGH); 
  Serial.begin(115200);
  delay(1000);
  SWire.begin();
  delay(1000);


  
  for (int i = 0; i < 10; i++)
  {
    rbuffer[i] = 0x00;
  }
  // Clear write buffer.
  for (int i = 0; i < 4; i++)
  {
    wbuffer[i] = 0x00;
  }
  SWire.requestFrom(addr,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    }
wbuffer[0] = B00000000;
wbuffer[1] = rbuffer[7] & B01111000;
wbuffer[2] = rbuffer[8];
wbuffer[3] = rbuffer[9] & B00011111;
for (int i = 0; i < sizeof(wbuffer); i++)
  {
    wbuffer[i] |= lpm4[i];
  }
  SWire.beginTransmission(addr);        
  SWire.write(wbuffer[0]); 
  SWire.write(wbuffer[1]);  
  SWire.write(wbuffer[2]); 
  SWire.write(wbuffer[3]);        
  SWire.endTransmission();



  
digitalWrite(PA0, HIGH);  
  for (int i = 0; i < 10; i++)
  {
    rbuffer[i] = 0x00;
  }
  // Clear write buffer.
  for (int i = 0; i < 4; i++)
  {
    wbuffer[i] = 0x00;
  }
  SWire.requestFrom(addr,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    Serial.print("\t");
    }
wbuffer[0] = B00000000;
wbuffer[1] = rbuffer[7] & B01111000;
wbuffer[2] = rbuffer[8];
wbuffer[3] = rbuffer[9] & B00011111;
for (int i = 0; i < sizeof(wbuffer); i++)
  {
    wbuffer[i] |= lpm3[i];
  }
  SWire.beginTransmission(addr);        
  SWire.write(wbuffer[0]); 
  SWire.write(wbuffer[1]);  
  SWire.write(wbuffer[2]); 
  SWire.write(wbuffer[3]);        
  SWire.endTransmission();



digitalWrite(PA1, HIGH);  
  for (int i = 0; i < 10; i++)
  {
    rbuffer[i] = 0x00;
  }

  // Clear write buffer.
  for (int i = 0; i < 4; i++)
  {
    wbuffer[i] = 0x00;
  }
  SWire.requestFrom(addr,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    Serial.print("\t");
    }
wbuffer[0] = B00000000;
wbuffer[1] = rbuffer[7] & B01111000;
wbuffer[2] = rbuffer[8];
wbuffer[3] = rbuffer[9] & B00011111;
for (int i = 0; i < sizeof(wbuffer); i++)
  {
    wbuffer[i] |= lpm2[i];
  }
  SWire.beginTransmission(addr);        
  SWire.write(wbuffer[0]); 
  SWire.write(wbuffer[1]);  
  SWire.write(wbuffer[2]); 
  SWire.write(wbuffer[3]);        
  SWire.endTransmission();


digitalWrite(PC2, HIGH);  
  for (int i = 0; i < 10; i++)
  {
    rbuffer[i] = 0x00;
  }
  // Clear write buffer.
  for (int i = 0; i < 4; i++)
  {
    wbuffer[i] = 0x00;
  }
  SWire.requestFrom(addr,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    Serial.print(rbuffer[i]);
    Serial.print("\t");
    }
wbuffer[0] = B00000000;
wbuffer[1] = rbuffer[7] & B01111000;
wbuffer[2] = rbuffer[8];
wbuffer[3] = rbuffer[9] & B00011111;
for (int i = 0; i < sizeof(wbuffer); i++)
  {
    wbuffer[i] |= lpm1[i];
  }
  SWire.beginTransmission(addr);        
  SWire.write(wbuffer[0]); 
  SWire.write(wbuffer[1]);  
  SWire.write(wbuffer[2]); 
  SWire.write(wbuffer[3]);        
  SWire.endTransmission();




  
  Serial.println("\nSoftware I2C.. Scanner");
  
 


  
}


void loop() {
 
Serial.println("\n");
  Serial.println("Scanning...");

SWire.requestFrom(0x4A,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    }
 Serial.print("\n");
 Serial.print("TLV1");
fun();



SWire.requestFrom(0x4E,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    }
 Serial.print("\n");
 Serial.print("TLV2");
fun();




SWire.requestFrom(0x5A,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    }
 Serial.print("\n");
 Serial.print("TLV3");
fun();








  
SWire.requestFrom(0x5E,sizeof(rbuffer));
  for(int i=0; i < sizeof(rbuffer); i++){
    rbuffer[i] = SWire.read();
    }
 Serial.print("\n");
 Serial.print("TLV4");
fun();



if(num==0)
{num++;
digitalWrite(PE5, HIGH);}
else
{num--;
digitalWrite(PE5, LOW); }
  

             // wait 5 seconds for next scan
delay(1000);
}

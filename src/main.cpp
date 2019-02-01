#include <Arduino.h>
#include <LedControl.h>
#include <SPI.h>

#define LED_HEARTBEAT 1

#if LED_HEARTBEAT
  #define HB_LED D2
  #define HB_LED_TIME 50 // in milliseconds
#endif

#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D7  // or MOSI
#define CS_PIN    D8  // or SS

int ctr;

int d = 30;  // animation delay


//  * Pin 12 is connected to the DATA IN-pin of the first MAX7221
//  * Pin 11 is connected to the CLK-pin of the first MAX7221
//  * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
// LedControl(dataPin, clockPin, csPin, numDevices) 
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1); 

/* Display a (hexadecimal) digit on a 7-Segment Display
 * Params:
 *  addr   address of the display
 *  digit  the position of the digit on the display (0..7)
 *  value  the value to be displayed. (0x00..0x0F)
 *  dp     sets the decimal point.  
*/  
void setDigit(int addr, int digit, byte value, boolean dp); 

// this function only prints up to -999 to 999 
void origprintNumber(int v) {  
    int ones;  
    int tens;  
    int hundreds; 

    boolean negative=false;

    if(v < -999 || v > 999)  
        return;  
    if(v<0) {  
        negative=true; 
        v=v*-1;  
    }
    ones=v%10;  
    v=v/10;  
    tens=v%10;  
    v=v/10; hundreds=v;  
    if(negative) {  
        //print character '-' in the leftmost column  
        lc.setChar(0,3,'-',false);  } 
    else {
        //print a blank in the sign column  
        lc.setChar(0,3,' ',false);  
    }  
    //Now print the number digit by digit 
    lc.setDigit(0,2,(byte)hundreds,false);
    lc.setDigit(0,1,(byte)tens,false); 
    lc.setDigit(0,0,(byte)ones,false); 
} 

// prints up to +99999999
void printNumber (int v) {  
    int d1; // ones  
    int d2; // tens
    int d3; // hundreds
    int d4; // thousands; 
    int d5; // ten-thousands
    int d6; // hundred-thousands
    int d7;
    int d8;

    // boolean negative = false;

    if(v < -99999999 || v > 99999999)  
        return;  

    // if(v<0) {  
    //     negative = true; 
    //     v = v * -1;  
    // }
  
    // extract each digit starting from rightmost
    d1 = v % 10; v = v / 10;  
    d2 = v % 10; v = v / 10; 
    d3 = v % 10; v = v / 10; 
    d4 = v % 10; v = v / 10; 
    d5 = v % 10; v = v / 10; 
    d6 = v % 10; v = v / 10; 
    d7 = v % 10; v = v / 10; 
    d8 = v % 10; v = v / 10; 
  
    // let's just print positive numbers, be positive, okay? no time or negativity
    // if (negative) {  
    //     //print character '-' in the leftmost column  
    //     lc.setChar(0, 3, '-', false);  } 
    // else {
    //     //print a blank in the sign column  
    //     lc.setChar(0, 3, ' ' , false);  
    // }  

    // print the number digit by digit
    // param: address, digit, byte value, decimal point?
    lc.setDigit(0, 7, (byte)d8, false);
    lc.setDigit(0, 6, (byte)d7, false);
    lc.setDigit(0, 5, (byte)d6, false);
    lc.setDigit(0, 4, (byte)d5, false);
    lc.setDigit(0, 3, (byte)d4, false);
    lc.setDigit(0, 2, (byte)d3, false);
    lc.setDigit(0, 1, (byte)d2, false); 
    lc.setDigit(0, 0, (byte)d1, false); 
} 


void snake1(){

  // right to left (top line)
  for (int i=0; i<8; i++){
    lc.setRow(0, i, 64);
    delay(d);
  }
  lc.setRow(0, 7, 66);
  delay(d);
  // draw middle lines
  lc.setRow(0, 7, 67);
  delay(d);

  // draw middle lines, left to right
  for (int i=6; i>=0; i--){
    lc.setRow(0, i, 65);
    delay(d);
  }

  // 1st col
  lc.setRow(0, 0, 81);
  delay(d);
  lc.setRow(0, 0, 89);
  delay(d);

  // bottom row, right to left
  for (int i=1; i<7; i++){
    lc.setRow(0, i, 73);
    delay(d);
  }

  // last digit
  lc.setRow(0, 7, 75);
  delay(d);

  // start of 
  // draw disappearing tails
  lc.setRow(0, 0, 25);
  delay(d);

  for (int i=1; i<7; i++){
    lc.setRow(0, i, 9);
    delay(d);
  }

  // draw 7th digit
  lc.setRow(0, 7, 11);
  delay(d);
  lc.setRow(0, 7, 9);
  delay(d);
  lc.setRow(0, 7, 8);
  delay(d);

  // draw tail left to right
  for (int i=6; i>=1; i--){
      lc.setRow(0, i, 8);
      delay(d);
  }

  // draw 1st digit 
  lc.setRow(0, 0, 24);
  delay(d);
  lc.setRow(0, 0, 8);
  delay(d);
  lc.setRow(0, 0, 0);
  delay(d);
  
  // draw tail, right to left lower line
  for (int i=1; i<8; i++){
    lc.setRow(0, i, 0);
    delay(d);
  }
}

void setup()
{
  #if LED_HEARTBEAT
    pinMode(HB_LED, OUTPUT);
    digitalWrite(HB_LED, LOW);
  #endif

  // switch to normal operation
  for (int index=0; index < lc.getDeviceCount(); index++) {
    lc.shutdown(index,false); 
  } 

  // wake up the MAX72XX from power-saving mode
  lc.shutdown(0, false); 
  
  // set brightness for the Leds (0..15)
  lc.setIntensity(0, 15); 

}

void loop()
{
  // blink our blue heartbeat LED
  #if LED_HEARTBEAT
    static uint32_t timeLast = 0;

    if (millis() - timeLast >= HB_LED_TIME)
    {
      digitalWrite(HB_LED, digitalRead(HB_LED) == LOW ? HIGH : LOW);
      timeLast = millis();
    }
  #endif

  // increment, and print
  // ctr++; 
  // printNumber(ctr); 

  // lc.setRow(0, 0, 0b00000001);
  // lc.setRow(0, 1, 0b00000010);
  // lc.setRow(0, 2, 0b00000100);
  // lc.setRow(0, 3, 0b00001000);
  // lc.setRow(0, 4, 0b00010000);
  // lc.setRow(0, 5, 0b00100000);
  // lc.setRow(0, 6, 0b01000000);
  // lc.setRow(0, 7, 0b10000000);

  snake1();

  // clear all 
  for (int i=0; i<8; i++){
    lc.setRow(0, i, 0);
  }

}
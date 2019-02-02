#include <Arduino.h>
#include <LedControl.h>
#include <SPI.h>

#define LED_HEARTBEAT 1

#if LED_HEARTBEAT
  #define HB_LED D2
  #define HB_LED_TIME 50 // in milliseconds
#endif

#define CLK_PIN   D5  // or SCKgit 
#define DATA_PIN  D7  // or MOSI
#define CS_PIN    D8  // or SS

int ctr;

int d = 5;  // animation delay

int VUMeter[8];
int VULeftValue;     // values 0..7
int VURightValue;    // values 0..7

//  * Pin 12 is connected to the DATA IN-pin of the first MAX7221
//  * Pin 11 is connected to the CLK-pin of the first MAX7221
//  * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
// LedControl(dataPin, clockPin, csPin, numDevices) 
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1); 


void UpdateVUMeterDisplay(){
  // update VU meter display
  for (int i=7; i>=0; i--){
    lc.setRow(0, i, VUMeter[i]);
    delay(d);  
  }
}

void ClearVUMeterDisplay(){
  for (int i=0; i<8; i++){
    lc.setRow(0, i, 0);
    delay(d*4);
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

  // initial value
  VULeftValue   = rand() % 8;
  VURightValue  = rand() % 8;

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

  VULeftValue   = rand() % 8;  // 0..7
  // populate VUMeter[] array with left channel values
  for (int i=7; i>=VULeftValue; i--){  
    VUMeter[i] = VUMeter[i] | 0b001000000; 
    lc.setRow(0, i, VUMeter[i]);
    delay(d);
  }
  // blank out rest of left channel leds 
  for (int i=0; i<VULeftValue-1; i++){
    VUMeter[i] = VUMeter[i] & 0b00001000;
    lc.setRow(0, i, VUMeter[i]);
    delay(d);
  }

  VURightValue   = rand() % 8;  // 0..7
  // populate VUMeter[] array with left channel values
  for (int i=7; i>=VURightValue; i--){  
    VUMeter[i] = VUMeter[i] | 0b00001000; 
    lc.setRow(0, i, VUMeter[i]);
    delay(d);
  }
  // blank out rest of left channel leds 
  for (int i=0; i<VURightValue-1; i++){
    VUMeter[i] = VUMeter[i] & 0b01000000;
    lc.setRow(0, i, VUMeter[i]);
    delay(d);
  }
  
  // UpdateVUMeterDisplay();
  // ClearVUMeterDisplay();
  
}
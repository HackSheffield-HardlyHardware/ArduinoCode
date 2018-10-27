#include "CurieIMU.h"
#include <Adafruit_NeoPixel.h>
#define PIN 9  //// what pin are the NeoPixels connected to?
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);  /// the strip is 15 pixels long.  You can change this for the number of pixels in your individual strip.



boolean blinkState = false;          // state of the LED

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while(!Serial) ;    // wait for serial port to connect..
  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 1); // 1.5g = 1500 mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50);   // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  Serial.println("IMU initialisation complete, waiting for events...");
  
  strip.begin();
  strip.show(); //Initialise all pixels off
}

void loop() {
  // blink the LED in the main loop:
  digitalWrite(13, blinkState);
  blinkState = !blinkState;
  TurnAllLedsOff();
  delay(1000);
}


static void eventCallback(void)
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    if (CurieIMU.shockDetected(Y_AXIS, POSITIVE))
    {
      TurnAllLedsOn(255,0,255);
      Serial.println("Negative shock detected on Y-axis");
    }
      
    if (CurieIMU.shockDetected(Y_AXIS, NEGATIVE))
    {
      TurnAllLedsOn(0,255,0);
      Serial.println("Positive shock detected on Y-axis");
    }

  }
}

void TurnAllLedsOn(int red,int green,int blue)
{
  for (int i = 0; i <30;i++)
  {
    strip.setPixelColor(i, red, green, blue);  
  }
  strip.show();
}

void TurnAllLedsOff()
{
  for (int i = 0; i <30;i++)
  {
    strip.setPixelColor(i, 0,0,0);   
  }
  strip.show();
}

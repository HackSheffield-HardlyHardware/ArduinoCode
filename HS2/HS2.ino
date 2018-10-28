#include <Arduino.h>
#include <U8g2lib.h>
#include "CurieTimerOne.h"


#include <Adafruit_NeoPixel.h>
#define PIN 9  //// what pin are the NeoPixels connected to?
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);  /// the strip is 30 pixels long.  You can change this for the number of pixels in your individual strip.


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1327_EA_W128128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


#include <AxisJoystick.h>
#define SW_PIN  5
#define VRX_PIN A1
#define VRY_PIN A2

AxisJoystick* joystick;
AxisJoystick::Move goal;
bool alarm = false;
int i = 0;
bool spkrOn = false;
int spkrPin = 3;
int count = 1000000/(2*4000);
int divi;
int target;

void oscillateBeeper() {
  if((divi % 2 || divi > 400) && divi < 800) {
    spkrOn = !spkrOn;
    digitalWrite(spkrPin,spkrOn);
  }
  if(divi++>20000) {
    divi = 0;
  }
}
void startAlarm() {
  CurieTimerOne.start(count, &oscillateBeeper);
}
void stopAlarm() {
    CurieTimerOne.stop();
}
AxisJoystick::Move chooseDirection(AxisJoystick::Move prevGoal) {
  AxisJoystick::Move nextDirection = prevGoal;
  while(nextDirection == prevGoal) {
    switch(random(4)) {
      case 0: 
        nextDirection = AxisJoystick::Move::UP;
        break;
      case 1: 
        nextDirection = AxisJoystick::Move::DOWN;
        break;
      case 2: 
        nextDirection = AxisJoystick::Move::RIGHT;
        break;
      case 3: 
        nextDirection = AxisJoystick::Move::LEFT;
        break;
      default: // bad path
        nextDirection = AxisJoystick::Move::NOT;
    }
  }
  return(nextDirection);
}

void drawUp()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0077);
  SetLEDS(0,255,0);
}

void drawDown()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0074);
  SetLEDS(0,255,255);

}
void drawLeft()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0075);
  SetLEDS(128,0,255);
}
void drawRight()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0076);
  SetLEDS(128,128,128);
}

void drawCross()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x010c);
  SetLEDS(255,0,0);
}


void SetLEDS(int red, int green , int blue)
{
    for (int i = 0; i <30;i++)
  {
    strip.setPixelColor(i, red, green, blue);  
  }
  strip.show();

}

void FlashLEDS()
{
  SetLEDS(255,0,0);
  delay(100);
  SetLEDS(0,0,0);
  delay(100);
  SetLEDS(255,0,0);
  delay(100);
  SetLEDS(0,0,0);
  
}

void drawArrow(AxisJoystick::Move goal) {
  u8g2.clearBuffer();
  switch(goal) {
    case AxisJoystick::Move::UP :
    drawUp();
    break;
    case AxisJoystick::Move::DOWN :
    drawDown();
    break;
    case AxisJoystick::Move::RIGHT :
    drawRight();
    break;
    case AxisJoystick::Move::LEFT:
    drawLeft();
    break;
    default: //bad
    drawCross();
    break;
  }
  u8g2.sendBuffer();
}
void playAlarm() {
  tone(3, 2000, 50);
  delay(100);
  tone(3,4000,50);
  

}
void setup() {
  pinMode(spkrPin, OUTPUT);
  goal = AxisJoystick::Move::NOT;
  // put your setup code here, to run once:
  joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
  u8g2.begin();
  Wire.setClock(40000L);
  u8g2.sendBuffer();
  goal = chooseDirection(goal);
  drawArrow(goal);
  strip.begin();
  strip.show(); //Initialise all pixels off
  target = 5;
}

void loop() {
  // put your main code here, to run repeatedly:
  const AxisJoystick::Move move = joystick->multipleRead();
  if(target > 0) {
    startAlarm();
    

    if(move == goal) {
      target--;
     goal = chooseDirection(goal);
    
     drawArrow(goal);
     while(joystick->multipleRead() != AxisJoystick::Move::NOT);
    } else if(move != AxisJoystick::Move::NOT) {
      FlashLEDS();
     while(joystick->multipleRead() != AxisJoystick::Move::NOT);
    }
  } else {
    stopAlarm();
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    for (int i = 0; i <30;i++)
    {
      strip.setPixelColor(i, 0, 255, 0);  
    }
    strip.show();
    if(joystick->multipleRead() != AxisJoystick::Move::PRESS) {
      target = 5;
    }
  }
}

#include <Arduino.h>
#include <U8g2lib.h>

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


AxisJoystick::Move chooseDirection() {
  switch(random(3)) {
    case 0: return(AxisJoystick::Move::UP);
    case 1: return(AxisJoystick::Move::DOWN);
    case 2: return(AxisJoystick::Move::RIGHT);
    case 3: return(AxisJoystick::Move::LEFT);
    default: // bad path
    return(AxisJoystick::Move::NOT);
  }
void setup() {
  // put your setup code here, to run once:
  joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
  u8g2.begin();

  Wire.setClock(40000L);

  u8g2.sendBuffer();
  
  strip.begin();
  strip.show(); //Initialise all pixels off
}

void drawUp()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0077);
  SetLEDS(255,0,0);
}

void drawDown()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0074);
  SetLEDS(0,255,0);

}
void drawLeft()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0075);
  SetLEDS(0,0,255);
}
void drawRight()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0076);
  SetLEDS(128,128,128);
}


void SetLEDS(int red, int green , int blue)
{
    for (int i = 0; i <30;i++)
  {
    strip.setPixelColor(i, red, green, blue);  
  }
  strip.show();

}
void drawArrow(AxisJoystick::Move goal) {
  switch(goal) {
    case AxisJoystick::Move::UP :
    drawUp();
    return;
    case AxisJoystick::Move::DOWN :
    drawDown();
    return;
    case AxisJoystick::Move::RIGHT :
    drawRight();
    return;
    case AxisJoystick::Move::LEFT:
    drawLeft();
    return;
    default: //bad
    //drawCross();
    break;
  }
}
void playAlarm() {
  tone(3, 2000, 50);
  delay(100);
  tone(3,4000,50);
  

}
void setup() {
  
  // put your setup code here, to run once:
  joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
  u8g2.begin();
  Wire.setClock(40000L);
  u8g2.sendBuffer();
  goal = chooseDirection();
  drawArrow(goal);
}

void loop() {
  // put your main code here, to run repeatedly:
  const AxisJoystick::Move move = joystick->multipleRead();

  if(move == AxisJoystick::Move::PRESS) {
    alarm = !alarm;
    i=0;
    u8g2.clearBuffer();
    if(alarm) {
      //drawOn();
    } else {
      //drawOff();
    }
    u8g2.sendBuffer();
    while(joystick->multipleRead() == AxisJoystick::Move::PRESS);
  }
  
  if(alarm && i == 0) {
    playAlarm();
    i = 50;
  }
  delay(50);
  if(i> 0) i--;
}

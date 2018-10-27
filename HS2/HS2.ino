#include <Arduino.h>
#include <U8g2lib.h>

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

bool alarm = false;
int i = 0;
void setup() {
  
  // put your setup code here, to run once:
  joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
  u8g2.begin();
  Wire.setClock(40000L);
  u8g2.sendBuffer();
}

void drawUp()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0077);
}

void drawDown()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0074);
}
void drawLeft()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0075);
}
void drawRight()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x0076);
}
void playAlarm() {
  tone(3, 2000, 50);
  delay(100);
  tone(3,4000,50);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  const AxisJoystick::Move move = joystick->multipleRead();
  if(move == AxisJoystick::Move::PRESS) {
    alarm = !alarm;
    i=0;
    u8g2.clearBuffer();
    if(alarm) {
      drawOn();
    } else {
      drawOff();
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

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
  drawOff();
  u8g2.sendBuffer();
}
void drawOn() {
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(5, 20, 0x23f6);
}
void drawOff() {
  u8g2.setFont(u8g2_font_unifont_t_symbols);
    DrawArrow();
}


void DrawArrow()
{

    int TopY = 30;
    int TopX = 30;
    int ArrowHeight = 50;
    int PointWidth = 30;
    int BottomY = TopY+ArrowHeight;
    int BottomX = TopX;
    int PointY = TopY + (ArrowHeight/2);
    int PointX = TopX - PointWidth;

    int BoxHeight = 30;
    int BoxWidth = 80;
    int BoxY = ((ArrowHeight - BoxHeight)/2) + TopY;
    int BoxX = TopX;
    
    u8g2.drawTriangle(TopY,TopX, BottomX,BottomY, PointX, PointY);//Top(x,y), Bottom(x,y), Point(x,y)
    u8g2.drawBox(BoxX,BoxY,BoxHeight,BoxWidth);// x,y,Height,Width
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
    tone(3, 2000, 50);
    delay(100);
    tone(3,4000,50);
    i = 50;
  }
  delay(50);
  if(i> 0) i--;
}

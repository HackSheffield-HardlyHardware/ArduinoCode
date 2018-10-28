#include <Arduino.h>
#include <U8g2lib.h>
#include <CurieBLE.h>

#include <Adafruit_NeoPixel.h>
#define PIN 9  //// what pin are the NeoPixels connected to?
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);  /// the strip is 30 pixels long.  You can change this for the number of pixels in your individual strip.

BLEPeripheral blePeripheral; // create peripheral instance
BLEService scoreReportService("19B10010-E8F2-537E-4F6C-D109432A1215"); // create service

BLEIntCharacteristic successCharacteristic("19B10010-E8F2-537E-4F6C-D109432A1215", BLERead | BLENotify); // allows remote device to get notifications  // Can be use like int but value may be read as Char

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
int progress = 0;


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

void drawCross()
{
  u8g2.setFont(u8g2_font_open_iconic_all_8x_t);
  u8g2.drawGlyph(30, 80, 0x010c);
  SetLEDS(255,0,0);
}

//void blankFirstLEDs(int LEDs)
//{
//  for (i = 0; i < LEDs; i++) 
//  {
//    strip.setPixelColor(i, 0, 0, 0);
//  }
//  strip.show();
//}

void SetLEDS(int red, int green , int blue)
{
    for (int i = progress; i <30;i++)
  {
    strip.setPixelColor(i, red, green, blue);  
  }
  strip.show();
}

//void SetLEDS(int red, int green , int blue)
//{
//    for (int i = 0; i <30;i++)
//  {
//    strip.setPixelColor(i, red, green, blue);  
//  }
//  strip.show();
//}


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
  
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
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

  // set the local name peripheral advertises
  blePeripheral.setLocalName("HardlyHacker");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(scoreReportService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(scoreReportService);
  blePeripheral.addAttribute(successCharacteristic);

  successCharacteristic.setValue(00);

  // advertise the bluetooth service
  blePeripheral.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  const AxisJoystick::Move move = joystick->multipleRead();

  if(move == goal) {
    progress++;
    successCharacteristic.setValue(progress);    //Report success to bluetooth
    Serial.print(progress);
    Serial.println();
    //alarm = !alarm;
    //i=0;
    goal = chooseDirection(goal);
    
    drawArrow(goal);
//    blankFirstLEDs(progress);

    while(joystick->multipleRead() != AxisJoystick::Move::NOT);
  }

//  strip.setPixelColor(progress, 0, 0, 0); // Use to record a fail?
  
  
  if(alarm && i == 0) {
    playAlarm();
    i = 50;
  }
  delay(50);
  if(i> 0) i--;
}

#include <Adafruit_NeoPixel.h>
#define PIN 9
#define N 30
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N, PIN, NEO_GRB + NEO_KHZ800);  /// the strip is 30 pixels long.  You can change this for the number of pixels in your individual strip.

int r=0;
int g=0;
int b=0;
int inc = 10;
void ripple() {
  int i;
  for(i = N;i>0;i--) {
    strip.setPixelColor(i,strip.getPixelColor(i-1));  
  }
}
void colourMe() {
  ripple();
  strip.setPixelColor(1,r,g,b);
  strip.show();
  //delay(1);
  delayMicroseconds(20000);
}
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  
}

void loop() {
  for(r=0;r<255;r= r + inc) {
    colourMe();
  }
  for(b=255;b>0;b=b-inc) {
    colourMe();
  }
  for(g=0;g<255;g=g+inc) {
    colourMe();
  }
  for(r=255;r>0;r=r-inc) {
    colourMe();
  }
  for(b=0;b<255;b=b+inc) {
    colourMe();
  }
  for(g=255;g>0;g=g-inc) {
    colourMe();
  }
}

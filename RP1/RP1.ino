
#include "CurieTimerOne.h"
bool spkrOn = false;
int spkrPin = 3;
int  count = 1000000/(2*4000);
int divi;
void oscillateBeeper() {
  if((divi % 2 || divi > 400) && divi < 800) {
    spkrOn = !spkrOn;
    digitalWrite(spkrPin,spkrOn);
  }
  if(divi++>20000) {
    divi = 0;
  }
}
void setup() {
  pinMode(spkrPin, OUTPUT);
  CurieTimerOne.start(count, &oscillateBeeper);
}

void loop() {
  delay(10000);
  CurieTimerOne.stop();
  delay(10000);
  CurieTimerOne.start(count, &oscillateBeeper);
}

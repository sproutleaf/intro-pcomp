#include <SPI.h>
#include <Shifty.h>

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;

int leds = 0;
Shifty shift;

void setup() {
  shift.setBitCount(16);
  shift.setPins(6, 4, 5);
}

void loop() {
  // writeBit works just like digitalWrite
  shift.writeBit(1, HIGH);
  delay(500);
  shift.writeBit(3, HIGH);
  delay(500);
  shift.writeBit(1, LOW);
  delay(500);
  shift.writeBit(3, LOW);
 
  delay(500);
}
#include "Shifter.h"

#define DATA_PIN 4 // DATA PIN
#define LATCH_PIN 5 // LATCH PIN
#define CLOCK_PIN 6 // CLOCK PIN
#define BUTTON_PIN 7

#define NUM_REGISTERS 2

Shifter shifter(DATA_PIN, LATCH_PIN, CLOCK_PIN, NUM_REGISTERS); 
int buttonState;
int delayMs = 100;
int randomInc;
bool slowDownActivated = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  for (int i = 0; i < 16; i++) {
    shifter.clear();
    shifter.setAll(HIGH);
    shifter.setPin(i, LOW);
    shifter.write();
    delay(delayMs);
    if (slowDownActivated) {
      randomInc = random(30, 70);
      delayMs += randomInc;

      // When delay is greater than or equal to 1000ms, do the following:
      // 1) Stop the light;
      // 2) Display the corresponding image / text on OLED;
      // 3) Reset slow down activated state.
      if (delayMs >= 1000) {
        buttonState = digitalRead(BUTTON_PIN);
        while (buttonState == LOW) {
          buttonState = digitalRead(BUTTON_PIN);
        }
        delayMs = 100;
        slowDownActivated = false;

        // Only exit the if statement when the buttonState becomes low again --
        // this way, even if someone is pressing the button for a long time, there
        // will not be mixup with the next iteration.
        while (buttonState == HIGH) {
          buttonState = digitalRead(BUTTON_PIN);
        }
      }
    } else {
      // We only want to read the button state when slow down has not been triggered.
      buttonState = digitalRead(BUTTON_PIN);
      // Once a button press is detected, we want to start the process of slowing down
      // the light movement. For this button, LOW = not pressed; HIGH = pressed.
      if (buttonState == HIGH) {
        slowDownActivated = true;
      }
    }
  }
}
#include "Shifter.h"
#include "Bitmap.h"
#include "ConwaysWayOfLife.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Variables for the shift registers
#define DATA_PIN 4   // DATA PIN
#define LATCH_PIN 5  // LATCH PIN
#define CLOCK_PIN 6  // CLOCK PIN

#define NUM_REGISTERS 3
Shifter shifter(DATA_PIN, LATCH_PIN, CLOCK_PIN, NUM_REGISTERS);

// Variables for the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Initializations
#define BUTTON_PIN 7
int buttonState;   // Either 0 for unpressed or 1 for pressed

// Initializations for the lights and counters
int delayMs = 50;  // Delay between LED lights when the ring is spinning
int gifDelayMs = 150;
unsigned long previousTime = millis();
unsigned long ledPreviousTime = millis();
unsigned long lastExitTime = millis();
int i = 0;
int j = 0;
int randomInc;  // Increment to delay once button is pressed
bool slowDownActivated = false;

void setup() {
  // Pressed = 1; unpressed = 0
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
}

void loop() {
  drawAnimation();
  blinkLights();
}

void drawAnimation() {
  unsigned long currentTime = millis();
  if (currentTime - previousTime > gifDelayMs) {
    previousTime = currentTime;
    display.clearDisplay();
    display.drawBitmap(0, 0, *(way_of_life_frames[j]), 128, 64, 1);
    display.display();
    j++;
    if (j == 30) {
      j = 0;
    }
  }
}

void blinkLights() {
  unsigned long ledCurrentTime = millis();
  if (ledCurrentTime - ledPreviousTime > delayMs) {
    Serial.println(delayMs);
    ledPreviousTime = ledCurrentTime;
    shifter.clear();
    shifter.setAll(HIGH);
    shifter.setPin(i, LOW);
    shifter.write();
    if (slowDownActivated) {
      randomInc = random(30, 70);
      delayMs += randomInc;

      // When delay is greater than or equal to 1000ms, do the following:
      // 1) Stop the light;
      // 2) Display the corresponding image / text on OLED;
      // 3) Reset slow down activated state.
      if (delayMs >= 1000) {
        if (i == 0 || i == 13) {
          drawPumpkin();          
        } else {
          displayResult(i);
        }

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
        lastExitTime = millis();
      }
    } else {
      unsigned long buttonCurrentTime = millis();
      // To prevent premature button slowdown into the next round.
      if (buttonCurrentTime - lastExitTime > delayMs) {
        // We only want to read the button state when slow down has not been triggered.
        buttonState = digitalRead(BUTTON_PIN);
        // Once a button press is detected, we want to start the process of slowing down
        // the light movement.
        if (buttonState == HIGH) {
          slowDownActivated = true;
        }
      }
    }
    // There's definitely a better way of doing this...
    i++;
    if (i == 24) {
      i = 0;
    }
  }
}

void drawPumpkin() {
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap, 128, 64, 1);
  display.display();
}

void displayResult(int i) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(50, 20);
  display.println(i + 1);
  display.invertDisplay(1);
  display.display();
}
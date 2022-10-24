#include "Shifter.h"
#include "Bitmap.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Variables for the shift registers
#define DATA_PIN 4 // DATA PIN
#define LATCH_PIN 5 // LATCH PIN
#define CLOCK_PIN 6 // CLOCK PIN

#define NUM_REGISTERS 3
Shifter shifter(DATA_PIN, LATCH_PIN, CLOCK_PIN, NUM_REGISTERS); 

// Variables for the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128

// Other initializations
#define BUTTON_PIN 7
int buttonState;  // Either 0 for unpressed or 1 for pressed
int delayMs = 100;  // Delay between LED lights when the ring is spinning
int randomInc;  // Increment to delay once button is pressed
bool slowDownActivated = false;

void setup() {
  // Pressed = 1; unpressed = 0
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  drawStartPumpkin(); // Draw the pumpkin for the beginning of the game
}

void loop() {
  for (int i = 0; i < NUM_REGISTERS * 8; i++) {
    shifter.clear();
    shifter.setAll(HIGH);
    shifter.setPin(i, LOW);
    shifter.write();
    delay(delayMs);
    Serial.println(delayMs);
    if (slowDownActivated) {
      Serial.println("Slowdown activated!");
      randomInc = random(30, 70);
      delayMs += randomInc;

      // When delay is greater than or equal to 1000ms, do the following:
      // 1) Stop the light;
      // 2) Display the corresponding image / text on OLED;
      // 3) Reset slow down activated state.
      if (delayMs >= 1000) {
        buttonState = digitalRead(BUTTON_PIN);
        while (buttonState == LOW) {
          Serial.println("Button not pressed!");
          buttonState = digitalRead(BUTTON_PIN);
        }
        delayMs = 100;
        slowDownActivated = false;

        // Only exit the if statement when the buttonState becomes low again --
        // this way, even if someone is pressing the button for a long time, there
        // will not be mixup with the next iteration.
        while (buttonState == HIGH) {
          Serial.println("Button pressed!");
          buttonState = digitalRead(BUTTON_PIN);
        }
      }
    } else {
      // We only want to read the button state when slow down has not been triggered.
      buttonState = digitalRead(BUTTON_PIN);
      Serial.println("Button state");
      Serial.println(buttonState);
      // Once a button press is detected, we want to start the process of slowing down
      // the light movement. For this button, LOW = not pressed; HIGH = pressed.
      if (buttonState == HIGH) {
        Serial.println("Button pressed!?");
        slowDownActivated = true;
      }
    }
  }
}

void drawStartPumpkin() {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    pumpkin, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}
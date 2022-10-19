#include "Servo.h"      // include the servo library
 
Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 9;       // Control pin for servo motor
// time when the servo was last updated, in ms
long lastMoveTime = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoMotor.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(A0); // read the analog input
  Serial.println(analogValue);      // print it
 
  // if your sensor's range is less than 0 to 1023, you'll need to
  // modify the map() function to use the values you discovered:
  int servoAngle = map(analogValue, 0, 1023, 0, 179);
  
  // move the servo using the angle from the sensor every 20 ms:
  if (millis() - lastMoveTime > 20) {
    servoMotor.write(servoAngle);
    lastMoveTime = millis();
  }
}
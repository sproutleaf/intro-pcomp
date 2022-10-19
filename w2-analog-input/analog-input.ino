const int ledPin = 9;       // pin that the LED is attached to
int analogValue = 0;        // value read from the pot
int brightness = 0;         // PWM pin that the LED is on.

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // declare the led pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogValue = analogRead(A0);    // read the pot value
  brightness = analogValue /4;       //divide by 4 to fit in a byte
  analogWrite(ledPin, brightness);   // PWM the LED with the brightness value
  Serial.println(brightness);        // print the brightness value back to t
}

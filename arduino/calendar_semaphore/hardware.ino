#include "SevSeg.h"
SevSeg sevSeg; // Initiate a seven segment controller object

const int greenLed = A5;
const int redLed = A4;

int timer = 60;  // Set your countdown start value (e.g., 100 seconds)
unsigned long previousMillis = 0;  // Will store the last time the timer was updated
const long interval = 1000;  // Interval for the countdown (1 second)

void setup() {
  byte numDigits = 4;  
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = 0; 
  // variable above indicates that 4 resistors were placed on the digit pins.
  // set variable to 1 if you want to use 8 resistors on the segment pins.
  sevSeg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevSeg.setBrightness(90);

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  // Turn on the red LED initially
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
}

void loop() {

  unsigned long currentMillis = millis();

  // Check if 1 second has passed (non-blocking delay)
  if (currentMillis - previousMillis >= interval && timer > 0) {
    previousMillis = currentMillis;  // Save the last time the timer was updated

    timer--;  // Decrement the timer
    sevSeg.setNumber(timer, 0);  // Display the timer
  }

  // Refresh the display continuously
  sevSeg.refreshDisplay();  // Must be called repeatedly

  // Check if the timer has reached zero to change LEDs
  if (timer <= 0) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
}

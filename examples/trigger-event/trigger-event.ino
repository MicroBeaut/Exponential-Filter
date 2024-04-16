#include "ExponentialFilter.h"

#define BUTTON_PIN A0

#define MIN_INPUT 0.0
#define MAX_INPUT 1.0

const float lowerThreshold = 0.293 * (MAX_INPUT - MIN_INPUT);
const float upperThreshold = 0.707 * (MAX_INPUT - MIN_INPUT);

ExpTrigger trigger {
  lowerThreshold, upperThreshold, GTLT
};

ExponentialFilter expButton;
bool toggle;

void OnTrigger(ExpEventArgs e);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  expButton.mode(AUTO);                   // Exponential Filter in Auto Mode
  expButton.cutoffTime(MS2S(20));         // Debounce Period = 20 milliseconds
  expButton.init(1.0);                    // INPUT/INPUT_PULLUP = 1.0
  expButton.eventOnTrigger(0, OnTrigger);
}

void loop() {
  expButton.filter(digitalRead(BUTTON_PIN));
  expButton.schmittTrigger(&trigger);
  if (trigger.upperRising) toggle = !toggle;
  digitalWrite(LED_BUILTIN, toggle);
}

void OnTrigger(ExpEventArgs e) {
  switch (e.state) {
    case LOWER_TRIGGER:
      Serial.print("EVENT:");
      Serial.print("LOWER");
      Serial.print(" ID:");
      Serial.println(e.id);
      break;
    case UPPER_TRIGGER:
      Serial.print("EVENT:");
      Serial.print("UPPER");
      Serial.print(" ID:");
      Serial.println(e.id);
      break;
  }
}
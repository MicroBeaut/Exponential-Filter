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

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  expButton.mode(AUTO);               // Exponential Filter in Auto Mode
  expButton.cutoffTime(MS2S(2000));   // Debounce Period = 20 milliseconds
  expButton.init(1.0);                // INPUT/INPUT_PULLUP = 1.0
}

void loop() {
  expButton.filter(!digitalRead(BUTTON_PIN));  // INPUT = INVERT INPUT
  expButton.schmittTrigger(&trigger);
  if (trigger.upperTrigger) toggle = !toggle;
  digitalWrite(LED_BUILTIN, toggle);
}
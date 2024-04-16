#include "ExponentialFilter.h"

#define BUTTON_PIN A0

#define MIN_INPUT 0.0
#define MAX_INPUT 1.0

const float lowerThreshold = 0.293 * (MAX_INPUT - MIN_INPUT);
const float upperThreshold = 0.707 * (MAX_INPUT - MIN_INPUT);

trigger_t trigger {
  lowerThreshold, upperThreshold, GTLT
};

ExponentialFilter expButton;
bool toggle;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  expButton.mode(AUTO);
  expButton.cutoffTime(MS2S(20));
  expButton.init(1.0);
}

void loop() {
  bool readValue = digitalRead(BUTTON_PIN);
  long value = readValue;
  expButton.expFilter(value);
  expButton.schmittTrigger(&trigger);
  if (trigger.upperRising) toggle = !toggle;
  digitalWrite(LED_BUILTIN, toggle);
}
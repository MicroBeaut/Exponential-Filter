#include "ExponentialFilter.h"

#define buttonPin A0
#define ledPin    10

ExponentialFilter expDebounce;
bool toggle;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  expDebounce.mode(AUTO);
  expDebounce.timeResponse(T2MS(3000));
  expDebounce.init(1.0);
}

void loop() {
  bool readValue = digitalRead(buttonPin);
  expDebounce.expFilter(readValue);
  double upper = 0.707;
  double lower = 0.293;
  expDebounce.trigger(upper, lower, LEGE, true);

  if (expDebounce.upperTrigger) {
    toggle = !toggle;
  }
  digitalWrite(ledPin, toggle);
  digitalWrite(LED_BUILTIN, expDebounce.trigger);

  Serial.println(
    "Input:" + String(readValue)
    + ", Output:" + String(expDebounce.output, 3)
    + ", Trigger:" + String(expDebounce.trigger)
    + ", Lower:" + String(lower, 3)
    + ", Upper:" + String(upper)
  );
}
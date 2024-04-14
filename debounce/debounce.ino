#include "ExponentialFilter.h"

#define sineInput A0
#define triangleInput A1
#define squareInput A2


#define buttonPin 9
#define timeInput A3

#define DEBOUNCE_PERIOD 10UL  //  Debounce Delay 10 milliseconds
typedef struct {
  private:
    bool input;
    unsigned long startTime;

  public:
    bool state;

    bool debounce(bool bounce) {
      unsigned long currentTime = millis();
      bool prevState = state;
      if (bounce) {
        state = true;
      } else {
        if (state) {
          if (input) {
            startTime = currentTime;
          }
          unsigned long elapsedTime = currentTime - startTime;
          if (elapsedTime >= DEBOUNCE_PERIOD) {
            state = false;
          }
        }
      }
      input = bounce;
      return state != prevState & state == true;
    }
} debounce_t;


ExponentialFilter lowPassFilter;
debounce_t button;

uint8_t state;

void setup() {
  Serial.begin(152000);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  lowPassFilter.timeConstant(0.010);
  randomSeed(analogRead(A4));
  Noise();
}

void loop() {
  double input = 0;
  bool value = !digitalRead(buttonPin);
  bool pressed = button.debounce(value);
  if (pressed) {
    state = (state + 1) % 3;
  }


  double noise = Noise();
  double sine = R2E(sineInput, 0, 5);
  double triangle = R2E(triangleInput, 0, 5);
  double square = R2E(squareInput, 0, 5);
  double t = R2E(timeInput, 10, 2000) * 0.001;
  double f = R2E(timeInput, 10, 3000) * 0.001;
  double rc = R2E(timeInput, 1.0, 31.4159265359) * 0.01;

  switch (state) {
    case 0:
      input = square;
      break;
    case 1:
      input = triangle;
      break;
    case 2:
      input = sine;
      break;
  }
  lowPassFilter.timeResponse(t);
  //lowPassFilter.frequency(f);
  //lowPassFilter.timeConstant(rc);
  lowPassFilter.filter(input);

  // bool trigger = lowPassFilter.schmittTrigger();
  SerialPlot(input, lowPassFilter.output, lowPassFilter.time, 0);
}

void SerialPlot(double input, double output, double time, bool trigger) {
  static unsigned long startTime;
  unsigned long currTime = millis();
  unsigned long elapsedTime = currTime - startTime;
  if (elapsedTime - startTime > 10UL) {
    startTime = currTime;
    Serial.println(
      "Input:" + String(input, 3)
      + ", Output:" + String(output, 3)
      + ", Trigger:" + String(trigger * 5.0F, 3)
      + ", Time:" + String(time * 5 / 1000, 3)
    );
  }
}

// RAW Value to Engineering Unit
double R2E(uint8_t pin, double min, double max) {
  long value = analogRead(pin);
  return value * max / 1023.0 + min;
}

double Noise() {
  static double noise;
  double prev = noise;
  while (prev == noise) {
    noise = random(-100, 00);
    noise = noise * 0.01F;
  }
  return noise;
}
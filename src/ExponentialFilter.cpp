#include "ExponentialFilter.h"

ExponentialFilter::ExponentialFilter():output(_output), time(_time) {
  timeResponse(1);
}

void ExponentialFilter::timeResponse(double secTime) {
  _time = secTime;
  _cutoffFrequency = 1 / _time;
  _timeConstant = _time / TWO_PI;
}

void ExponentialFilter::frequency(double freq) {
  _cutoffFrequency = freq;
  _time = 1 / _cutoffFrequency;
  _timeConstant = _time / TWO_PI;
}

void ExponentialFilter::timeConstant(double rc) {
  _timeConstant = rc;
  _time = TWO_PI * rc;
  _cutoffFrequency = 1 / _time;
}

double ExponentialFilter::filter(double input) {
  unsigned long currTime = micros();
  unsigned long elapsedTime = currTime - _lastTime;
  _lastTime = currTime;
  _deltaTime = elapsedTime * 0.000001;
  _alpha = _deltaTime / (_timeConstant + _deltaTime);
  _output += _alpha * (input - _output);
  return 0.0;
}

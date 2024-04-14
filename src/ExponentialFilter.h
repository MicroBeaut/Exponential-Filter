#ifndef EXPONENTIALFILTER_H
#define EXPONENTIALFILTER_H

#include <Arduino.h>

#define ONE_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307179586476925286766559


class ExponentialFilter {
private:
  double _timeConstant;     // τ = RC
  double _cutoffFrequency;  // ƒc
  double _smoothingFactor;  // α = ∆ₜ / (RC + ∆ₜ)
  double _time;
  double _alpha;
  double _output;

  double _deltaTime;
  unsigned long _lastTime;
public:
  double &output;
  double &time;

  ExponentialFilter(/* args */);
  void timeResponse(double secTime);
  void frequency(double freq);
  void timeConstant(double rc);
  double filter(double input);

};

#endif // EXPONENTIALFILTER_H
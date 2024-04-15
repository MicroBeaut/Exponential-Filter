#include "ExponentialFilter.h"

/// @brief
ExponentialFilter::ExponentialFilter(): output(_output), trigger(_trigger), lowerTrigger(_lowerTrigger), upperTrigger(_upperTrigger) {
  timeResponse(1);
}

/// @brief
/// @param output
void ExponentialFilter::init(double output) {
  _output = output;
}


/// @brief
/// @param time
void ExponentialFilter::timeResponse(double time) {
  _timeConstant = time / TWO_PI;
}

/// @brief
/// @param cutoffFrequency
void ExponentialFilter::frequency(double cutoffFrequency) {
  _timeConstant = 1 / (TWO_PI * cutoffFrequency);
}

/// @brief
/// @param rc
void ExponentialFilter::timeConstant(double rc) {
  _timeConstant = rc;
}

/// @brief
/// @param mode
void ExponentialFilter::mode(bool mode) {
  _mode = mode;
}

/// @brief
/// @param input
/// @return
double ExponentialFilter::expFilter(double input) {
  unsigned long currTime = micros();
  unsigned long elapsedTime = currTime - _lastTime;
  _lastTime = currTime;
  double _deltaTime = elapsedTime * 0.000001;
  if (_mode) {
    double _alpha = _deltaTime / (_timeConstant + _deltaTime);
    if (_alpha > 0.0 and _alpha <= 1.0) {
      _output += _alpha * (input - _output);
    }
  } else {
    _output = input;
  }
  return _output;
}

/// @brief
/// @param lowerTrigger
/// @param upperTrigger
/// @return
bool ExponentialFilter::schmittTriggers(double upperTrigger, double lowerTrigger, doubleOperation_t operation, bool triggerState) {
  _lowerTrigger = false;
  _upperTrigger = false;
  bool prevTrigger = _trigger;
  _trigger = compare(upperTrigger, lowerTrigger, operation, triggerState);
  _upperTrigger = _trigger == !triggerState & prevTrigger == triggerState;
  _lowerTrigger = _trigger == triggerState & prevTrigger == !triggerState;

  return _trigger;
}

bool ExponentialFilter::compare(double upperTrigger, double lowerTrigger, doubleOperation_t operation, bool triggerState) {
  if (comparator(comparation[operation][0], _output, upperTrigger)) return !triggerState;
  if (comparator(comparation[operation][1], _output, lowerTrigger)) return triggerState;
  return _trigger;
}

bool ExponentialFilter::comparator(operation_t operation, double a, double b) {
  switch (operation) {
    case LT:
      return a < b;
      break;
    case GT:
      return a > b;
      break;
    case LE:
      return a <= b;
      break;
    case GE:
      return a >= b;
      break;
    default:
      return false;
      break;
  }
}
/*
  MIT License
  Copyright (c) 2024 MicroBeaut
*/

#include "ExponentialFilter.h"

ExponentialFilter::ExponentialFilter(): output(_output), input(_input) {
  cutoffTime(1);
  _mode = AUTO;
}


void ExponentialFilter::init(float input) {
  _output = (float)input;
}


void ExponentialFilter::cutoffTime(float cutoffTime) {
  _timeConstant = cutoffTime / TWO_PI;
}


void ExponentialFilter::cutoffFrequency(float cutoffFrequency) {
  _timeConstant = 1 / (TWO_PI * cutoffFrequency);
}


void ExponentialFilter::timeConstant(float timeConstant) {
  _timeConstant = timeConstant;
}


void ExponentialFilter::mode(bool mode) {
  _mode = mode;
}


float ExponentialFilter::expFilter(float input) {
  unsigned long currTime = micros();
  unsigned long elapsedTime = currTime - _lastTime;
  _lastTime = currTime;
  float _deltaTime = elapsedTime * 0.000001;
  _input = input;
  if (_mode == AUTO) {
    float _alpha = _deltaTime / (_timeConstant + _deltaTime);
    if (_alpha > 0.0 and _alpha <= 1.0) {
      _output += _alpha * (_input - _output);
    }
  } else {
    _output = _input;
  }
  return _output;
}


bool ExponentialFilter::schmittTrigger(trigger_t *trigger) {
  trigger->lowerRising = false;
  trigger->upperRising = false;
  bool prevTrigger = trigger->trigger;
  trigger->trigger = internalTriggerCompare(trigger);
  trigger->upperRising = trigger->trigger == true & prevTrigger == false;
  trigger->lowerRising = trigger->trigger == false & prevTrigger == true;
  return trigger->trigger;
}

bool ExponentialFilter::internalTriggerCompare(trigger_t *trigger) {
  if (internalCompare( _output, trigger->upperThreshold, comparation[trigger->operation][1])) return true;
  if (internalCompare(_output, trigger->lowerThreshold, comparation[trigger->operation][0])) return false;
  return trigger->trigger;
}

bool ExponentialFilter::internalCompare(float a, float b, operation_t operation) {
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
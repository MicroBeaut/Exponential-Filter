/*
  MIT License
  Copyright (c) 2024 MicroBeaut
*/

#include "ExponentialFilter.h"

ExponentialFilter::ExponentialFilter(): output(_output), input(_input), id(_id) {
  cutoffTime(1);
  _mode = AUTO;
}


void ExponentialFilter::init(float input) {
  _output = (float)input;
}

void ExponentialFilter::eventOnTrigger(uint8_t id, ExpCallbackFunction function) {
  _id = id;
  _function = function;
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


float ExponentialFilter::filter(float input) {
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


bool ExponentialFilter::schmittTrigger(ExpTrigger *trigger) {
  trigger->lowerTrigger = false;
  trigger->upperTrigger = false;
  bool prevTrigger = trigger->trigger;
  trigger->trigger = internalTriggerCompare(trigger);
  trigger->upperTrigger = trigger->trigger == true & prevTrigger == false;
  trigger->lowerTrigger = trigger->trigger == false & prevTrigger == true;
  internalOnTrigger(trigger);
  return trigger->trigger;
}


void ExponentialFilter::internalOnTrigger(ExpTrigger *trigger) {
  if (_function) {
    ExpEventArgs e = {id: _id, input: _input, output: _output, timeConstant: _timeConstant};
    if (trigger->lowerTrigger) {
      e.state = LOWER_TRIGGER;
      _function(e);
    } else if (trigger->upperTrigger) {
      e.state = UPPER_TRIGGER;
      _function(e);
    }
    
    e.state = NONE_TRIGGER;
  }
}


bool ExponentialFilter::internalTriggerCompare(ExpTrigger *trigger) {
  if (internalCompare( _output, trigger->upperThreshold, mapOperations[trigger->operation][1])) return true;
  if (internalCompare(_output, trigger->lowerThreshold, mapOperations[trigger->operation][0])) return false;
  return trigger->trigger;
}


bool ExponentialFilter::internalCompare(float a, float b, ExpOperations operation) {
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
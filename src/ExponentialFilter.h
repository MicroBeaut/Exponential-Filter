/*
  MIT License
  Copyright (c) 2024 MicroBeaut
*/

#ifndef EXPONENTIALFILTER_H
#define EXPONENTIALFILTER_H

#include <Arduino.h>

#ifndef TWO_PI
#define TWO_PI 6.283185307179586476925286766559
#endif  // TWO_PI

#define FULLY_CHARGED     99.3    // Pencent
#define FULLY_DISCHARGED  0.7     // Pencent
#define CUTOFF            70.7    // Pencent
#define TRACTIONAL        63.2    // Pencent

#define S2MS(sec) (sec * 1000)
#define S2US(sec) (sec * 1000000)
#define MS2S(ms) (ms * 0.001)
#define US2S(us) (us * 0.000001)

enum ExpModes {
  AUTO,
  MANUAL
};

enum ExpOperations {
  LT,
  GT,
  LE,
  GE
};

enum ExpDoubleOperations {
  LTGT,
  GTLT,
  LEGE,
  GELE
};

const ExpOperations mapOperations[4][2] = {
  {LT, GT}, // LTGT
  {GT, LT}, // GTLT
  {LE, GE}, // LEGE
  {GE, LE}  // GELE
};

typedef struct {
  float lowerThreshold;
  float upperThreshold;
  ExpDoubleOperations operation;

  bool trigger;
  bool lowerRising;
  bool upperRising;
} ExpTrigger;


class ExponentialFilter {
  private:
    float _timeConstant;
    float _input;
    float _output;
    bool _mode;

    unsigned long _lastTime;

    bool internalTriggerCompare(ExpTrigger *trigger);
    bool internalCompare(float a, float b, ExpOperations operation);
  public:
    float &input;
    float &output;

    ExponentialFilter();
    void cutoffTime(float time);
    void cutoffFrequency(float cutoffFrequency);
    void timeConstant(float timeConstant);
    void mode(bool mode);
    void init(float input);
    float filter(float input);
    bool schmittTrigger(ExpTrigger *trigger);
};

#endif // EXPONENTIALFILTER_H
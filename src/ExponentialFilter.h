#ifndef EXPONENTIALFILTER_H
#define EXPONENTIALFILTER_H

#include <Arduino.h>

#define ONE_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307179586476925286766559
#define FULLY_CHARGED     0.993   // Pencent
#define FULLY_DISCHARGED  0.007   // Pencent
#define TRACTIONAL        0.632   // Pencent

#define T2MS(sec) (sec * 0.001)
#define T2US(sec) (sec * 0.000001)

enum mode_t : bool {
  MANUAL,
  AUTO
};

enum trigger_t : uint8_t {
  LAST_TRIGGER,
  LOWER_TRIGGER,
  UPPER_TRIGGER
};

enum doubleOperation_t : uint8_t {
  LTGT,
  GTLT,
  LEGE,
  GELE,
  LTLT,
  GTGT,
  LTGE,
  LEGT
};

enum operation_t : uint8_t {
  LT,
  GT,
  LE,
  GE
};

const operation_t comparation[8][2] = {
  {LT, GT},
  {GT, LT},
  {LE, GE},
  {GE, LE},
  {LT, LT},
  {GT, GT},
  {LT, GE},
  {LE, GT}
};

/// @brief fdafd
class ExponentialFilter {
  private:
    double _timeConstant;     // τ = RC
    double _output;
    bool _mode;
    bool _trigger;
    bool _lowerTrigger;
    bool _upperTrigger;

    unsigned long _lastTime;
    bool compare(double upperTrigger = FULLY_CHARGED, double lowerTrigger = FULLY_DISCHARGED, doubleOperation_t operation = LTGT, bool triggerState = false);
    bool comparator(operation_t operation, double a, double b);
  
  public:
    double &output;
    bool &trigger;
    bool &lowerTrigger;
    bool &upperTrigger;

    ExponentialFilter();
    void timeResponse(double time);
    void frequency(double cutoffFrequency);
    void timeConstant(double rc);
    void mode(bool mode);
    void init(double output);
    double expFilter(double input);
    bool schmittTriggers(double upperTrigger = FULLY_CHARGED, double lowerTrigger = FULLY_DISCHARGED, doubleOperation_t operation = LTGT, bool triggerState = false);
};

#endif // EXPONENTIALFILTER_H
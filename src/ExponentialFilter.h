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

#define S2MS(sec) (sec * 1000)
#define S2US(sec) (sec * 1000000)
#define MS2S(ms) (ms * 0.001)
#define US2S(us) (us * 0.000001)

/// @brief The Trigger states
enum ExpModes {
  AUTO,
  MANUAL
};


/// @brief The State Event Arguments
enum StateEventArgs {
  /// @brief Maintained the last state
  NONE_TRIGGER,
  /// @brief The output has met the lower trigger setting.
  LOWER_TRIGGER,
  /// @brief The output has met the upper trigger setting.
  UPPER_TRIGGER
};


enum ExpOperations {
  /// @brief Less Than
  LT,
  /// @brief Greater Than
  GT,
  /// @brief Less than or Equal to
  LE,
  /// @brief Greater than or Equal to
  GE
};


/// @brief Comparing the upper and lower thresholds for a trigger using exponential double operations.
enum ExpDoubleOperations {
  /// @brief If the output is less than the upper threshold setting, set the trigger state to false;
  /// @brief if output is greater than the lower threshold setting, set the trigger state to true.
  LTGT,
  /// @brief If the output is greater than the upper threshold setting, set the trigger state to false;
  /// @brief if output is less than the lower threshold setting, set the trigger state to true.
  GTLT,
  /// @brief If the output is less than or equal to the upper threshold setting, set the trigger state to false;
  /// @brief if the output is greater than or equal to the lower threshold setting, set the trigger state to true.
  LEGE,
  /// @brief If the output is greater than or equal to the upper threshold setting, set the trigger state to false;
  /// @brief if output is less than or equal to the lower threshold setting, set the trigger state to true.
  GELE
};


/// @brief The index compares a trigger's upper and lower thresholds using exponential double operations.
const ExpOperations mapOperations[4][2] = {
  {LT, GT}, // LTGT
  {GT, LT}, // GTLT
  {LE, GE}, // LEGE
  {GE, LE}  // GELE
};


/// @brief The Exponential Trigger Members
typedef struct {
  /// @brief Lower Threshold Setting
  float lowerThreshold;
  /// @brief Upper Threshold Setting
  float upperThreshold;
  /// @brief Comparing the upper and lower thresholds operations.
  ExpDoubleOperations operation;
  /// @brief Trigger State
  bool trigger;
  /// @brief Lower Trigger State
  bool lowerTrigger;
  /// @brief Upper Trigger State
  bool upperTrigger;
} ExpTrigger;


/// @brief Exponential Event Argument
typedef struct {
  /// @brief Exponential Id
  uint8_t id;
  /// @brief Exponential Input
  float input;
  /// @brief Exponential Output
  float output;
  /// @brief Exponential Time Const (RC)
  float timeConstant;
  /// @brief The State Event Arguments
  StateEventArgs state;
} ExpEventArgs;


typedef void (*ExpCallbackFunction)(ExpEventArgs);
class ExponentialFilter {
  private:
    float _timeConstant;
    float _input;
    float _output;
    bool _trigger;
    bool _mode;
    uint8_t _id;
    ExpCallbackFunction _function;

    unsigned long _lastTime;

    bool internalTriggerCompare(ExpTrigger *trigger);
    bool internalCompare(float a, float b, ExpOperations operation);
    void internalOnTrigger(ExpTrigger *trigger);
  public:
    float &input;
    float &output;
    bool &trigger;
    uint8_t &id;

    /// @brief Initial Exponential Filter
    /// @brief The time constant value set by default is 1.
    ExponentialFilter();

    /// @brief The proposed option allows for the setting of the time constant based on the cutoff time in an exponential manner.
    /// @brief cutoff time = 1/cutoffFrequency
    /// @param cutoffTime
    void cutoffTime(float cutoffTime);

    /// @brief The proposed option allows for the setting of the time constant based on the cutoff frequency in an exponential manner.
    /// @param cutoffFrequency
    void cutoffFrequency(float cutoffFrequency);

    /// @brief The proposed option allows for the setting of the time constant based on the time constant in an exponential manner.
    /// @param timeConstant
    void timeConstant(float timeConstant);

    /// @brief The mode selection is responsible for filtering the input, and by default, it is set to AUTO mode.
    /// @brief If the mode is switched to MANUAL mode, the output is equal to the input.
    /// @param mode
    void mode(bool mode);

    /// @brief The suggested alternative provides the feasibility of commencing the output with a predetermined value.
    /// @param input
    void init(float input);

    /// @brief Implemented the event callback on the trigger changed with the provided ID for the option that features the array input/output.
    /// @param id
    /// @param function
    void eventOnTrigger(uint8_t id, ExpCallbackFunction function);

    /// @brief Exponential filters smooth noisy input signals in signal processing by suppressing noise and producing a stable output
    /// @param input
    /// @return a stable output in a float data type
    float filter(float input);

    /// @brief The Schmitt Trigger compares the output to upper and lower thresholds and has multiple operation options to determine its trigger state.
    /// @param *trigger
    /// @return a trigger state
    bool schmittTrigger(ExpTrigger *trigger);
};

#endif // EXPONENTIALFILTER_H
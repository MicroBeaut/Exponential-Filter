# Exponential-Filter Library for Arduino

[![Arduino-Lint-Action](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Arduino-Lint-Action.yml/badge.svg)](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Arduino-Lint-Action.yml)
[![Spell-Check](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Spell-Check.yml/badge.svg)](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Spell-Check.yml)
[![Compile-Examples](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Compile-Examples.yml/badge.svg)](https://github.com/MicroBeaut/Exponential-Filter/actions/workflows/Compile-Examples.yml)
![Github](https://img.shields.io/github/v/release/MicroBeaut/Exponential-Filter)


Arduino Exponential-Filter provides an exponential filter to smooth a "noisy" input data value.

```
OUTPUT[t] = OUTPUT[t-1] + α(INPUT[t] - OUTPUT[t-1])
         += α(INPUT[t] - OUTPUT[t-1])

Where:
α   = ∆t/ (RC + ∆t)

When:
τ = RC [seconds]
  = 1 / 2πƒ
```
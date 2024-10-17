#ifndef SCHIEBER_H
#define SCHIEBER_H

#include <Arduino.h>
#include "BasePosServo.h"

class Schieber : public BasePosServo
{
public:
    // Constructor for the Schere class
    Schieber(int posPin, int negPin, int adcPin, int tol, const int positions[2]);
    // Overriding the parseInput method to control the actuator
    ModuleState parseInput(int inputValue) override;
};

#endif // SCHIEBER_H

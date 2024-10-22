#ifndef SCHIEBER_H
#define SCHIEBER_H

#include <Arduino.h>
#include "BasePosServo.h"

class Schieber : public BasePosServo
{
public:
    // Constructor for the Schere class
    Schieber(int posPin, int negPin, int adcPin, uint16_t tol, const int positions[2], uint16_t closedLoopTol = 0, bool inverseMapping = false);
    // Overriding the parseInput method to control the actuator
    ModuleState parseInput(int inputValue) override;
};

#endif // SCHIEBER_H

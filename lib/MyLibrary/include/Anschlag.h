#ifndef ANSCHLAG_H
#define ANSCHLAG_H

#include <Arduino.h>
#include "BasePosServo.h"

class Anschlag : public BasePosServo
{
public:
    // Constructor
    Anschlag(int posPin, int negPin, int adcPin, uint16_t tol, uint16_t closedLoopTol = 0, bool inverseMapping = false);
    // Constructor
    Anschlag(int posPin, int negPin, int adcPin, uint16_t tol, const int positions[2], uint16_t _closedLoopTol = 0, bool inverseMapping = false);
    // Copy Constructor
    Anschlag(const Anschlag& other) = default;

    // Parse input commands from the master and trigger the corresponding function
    ModuleState parseInput(int inputValue) override;
};

#endif // ANSCHLAG_H

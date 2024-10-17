#ifndef SCHERE_H
#define SCHERE_H

#include <Arduino.h>
#include "BasePosServo.h"

class Schere : public BasePosServo
{
public:
    // Constructor for the Schere class
    Schere(int posPin, int negPin, int adcPin, int tol, const int positions[2]);
    // Overriding the parseInput method to control the actuator
    ModuleState parseInput(int inputValue) override;
};

#endif // SCHERE_H

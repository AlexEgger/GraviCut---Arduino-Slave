#ifndef ANSCHLAG_H
#define ANSCHLAG_H

#include <Arduino.h>
#include "BasePosServo.h"

class Anschlag : public BasePosServo
{
public:
    // Constructor
    Anschlag(int posPin, int negPin, int adcPin, int tol);

    // Parse input commands from the master and trigger the corresponding function
    ModuleState parseInput(int inputValue) override;
};

#endif // ANSCHLAG_H

#ifndef ANSCHLAG_H
#define ANSCHLAG_H

#include <Arduino.h>
#include "BasePosServo.h"

class Anschlag : public BasePosServo
{
private:
    // Move actuator to a specific position
    int Position(int x);

public:
    // Constructor
    Anschlag(int posPin, int negPin, int adcPin, int tol);

    // Parse input commands from the master and trigger the corresponding function
    int parseInput(int inputValue) override;
};

#endif // ANSCHLAG_H

#ifndef SCHIEBER_H
#define SCHIEBER_H

#include <Arduino.h>
#include "BasePosServo.h"

class Schieber : public BasePosServo
{
private:
    // Position A
    int _posA;

    // Position B
    int _posB;

    // Move to Position A
    int PosA();

    // Move to Position B
    int PosB();

public:
    // Constructor
    Schieber(int posPin, int negPin, int adcPin, int tol, int posA, int posB); // posA < posB

    // Overriding the parseInput method to control the actuator
    int parseInput(int inputValue) override;
};

#endif // SCHIEBER_H

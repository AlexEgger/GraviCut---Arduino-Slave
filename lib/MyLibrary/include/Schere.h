#ifndef SCHERE_H
#define SCHERE_H

#include <Arduino.h>
#include "BasePosServo.h"

class Schere : public BasePosServo
{
private:
    // Position for Open state
    int _posOpen;

    // Position for Closed state
    int _posClosed;

    // Private methods to control the actuator
    int Open();
    int Close();

public:
    // Constructor
    Schere(int posPin, int negPin, int adcPin, int tol, int posOpen, int posClosed); // posOpen > posClosed

    // Overriding the parseInput method to control the actuator
    int parseInput(int inputValue) override;
};

#endif // SCHERE_H

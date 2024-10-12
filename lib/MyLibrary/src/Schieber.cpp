#include "Schieber.h"

// Constructor
Schieber::Schieber(int posPin, int negPin, int adcPin, int tol, int posA, int posB)
    : BasePosServo(posPin, negPin, adcPin, tol), // Calling the base class constructor
      _posA(posA),                               // Initialize Position A
      _posB(posB)                                // Initialize Position B
{
    // No additional initialization needed
}

// Overriding the parseInput method to control the actuator
int Schieber::parseInput(int inputValue)
{
    if (inputValue == 0) // Move to Position A
    {
        return PosA();
    }
    else if (inputValue == 1) // Move to Position B
    {
        return PosB();
    }
    return -1; // Invalid input
}

// Move to Position A
int Schieber::PosA()
{
    updatePosition();
    if (_position < _posA) // Compare current position with Position A
    {
        digitalWrite(_posPin, HIGH);
        digitalWrite(_negPin, LOW);
        return 0; // Still running
    }
    else
    {
        digitalWrite(_posPin, LOW);
        digitalWrite(_negPin, LOW);
        return 1; // Operation completed
    }
}

// Move to Position B
int Schieber::PosB()
{
    updatePosition();
    if (_position > _posB) // Compare current position with Position B
    {
        digitalWrite(_posPin, HIGH);
        digitalWrite(_negPin, LOW);
        return 0; // Still running
    }
    else
    {
        digitalWrite(_posPin, LOW);
        digitalWrite(_negPin, LOW);
        return 1; // Operation completed
    }
}

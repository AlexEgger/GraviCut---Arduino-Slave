#include "Anschlag.h"

// Constructor
Anschlag::Anschlag(int posPin, int negPin, int adcPin, int tol)
    : BasePosServo(posPin, negPin, adcPin, tol) // Calling the base class constructor
{
    // No additional initialization needed
}

// Parse input commands from the master and trigger the corresponding function
int Anschlag::parseInput(int inputValue)
{
    return Position(inputValue); // Position to move to based on input
}

// Move actuator to a specific position
int Anschlag::Position(int x)
{
    updatePosition(); // Position aktualisieren
    if (_position < x - _tol)
    {
        digitalWrite(_posPin, HIGH);
        digitalWrite(_negPin, LOW);
        return 0; // Still running
    }
    else if (_position > x + _tol)
    {
        digitalWrite(_posPin, LOW);
        digitalWrite(_negPin, HIGH);
        return 0; // Still running
    }
    else
    {
        digitalWrite(_posPin, LOW);
        digitalWrite(_negPin, LOW);
        return 1; // Operation completed
    }
}

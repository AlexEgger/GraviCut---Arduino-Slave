#include "Schere.h"

// Constructor
Schere::Schere(int posPin, int negPin, int adcPin, int tol, int posOpen, int posClosed)
    : BasePosServo(posPin, negPin, adcPin, tol), // Calling the base class constructor
      _posOpen(posOpen), // Initialize Open position
      _posClosed(posClosed) // Initialize Closed position
{
    // No additional initialization needed
}

// Overriding the parseInput method to control the actuator
int Schere::parseInput(int inputValue)
{
    if (inputValue == 0) // Close command
    {
        return Close();
    }
    else if (inputValue == 1) // Open command
    {
        return Open();
    }
    return -1; // Invalid input
}

// Private methods to control the actuator
int Schere::Open()
{
    updatePosition();
    if (_position < _posOpen) // Compare with the defined open position
    {
        digitalWrite(_negPin, LOW);
        digitalWrite(_posPin, HIGH);
        return 0; // Still running
    }
    else
    {
        digitalWrite(_posPin, LOW);
        digitalWrite(_negPin, LOW);
        return 1; // Operation completed
    }
}

int Schere::Close()
{
    updatePosition();
    if (_position > _posClosed) // Compare with the defined closed position
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

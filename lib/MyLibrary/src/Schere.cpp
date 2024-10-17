#include "Schere.h"

// Constructor
Schere::Schere(int posPin, int negPin, int adcPin, int tol, const int positions[2])
    : BasePosServo(posPin, negPin, adcPin, tol, positions) // Calling the base class constructor
{
    // No additional initialization needed
}

// Overriding the parseInput method to control the actuator
ModuleState Schere::parseInput(int inputValue)
{
    if (inputValue == 0) // Move to Position A
    {
        return positionMaximum(_storedPositions[0]);
    }
    else if (inputValue == 1) // Move to Position B
    {
        return positionMinimum(_storedPositions[1]);
    }
    return InvalidState; // Invalid input
}

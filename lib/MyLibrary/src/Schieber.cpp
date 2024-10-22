#include "Schieber.h"

// Constructor
Schieber::Schieber(int posPin, int negPin, int adcPin, uint16_t tol, const int positions[2], uint16_t closedLoopTol, bool inverseMapping)
    : BasePosServo(posPin, negPin, adcPin, tol, positions, closedLoopTol, inverseMapping) // Calling the base class constructor
{
    // No additional initialization needed
}

// Overriding the parseInput method to control the actuator
ModuleState Schieber::parseInput(int inputValue)
{
    if (inputValue == 0) // Move to Position A
    {
        return positionTarget(_storedPositions[0]);
    }
    else if (inputValue == 1) // Move to Position B
    {
        return positionTarget(_storedPositions[1]);
    }
    return ErrorState; // Invalid input
}

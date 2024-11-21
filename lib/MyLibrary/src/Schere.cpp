#include "Schere.h"

// Constructor
Schere::Schere(int posPin, int negPin, int adcPin, uint16_t tol, uint16_t closedLoopTol, bool inverseMapping, bool calibrateMinMax)
    : BasePosServo(posPin, negPin, adcPin, tol, closedLoopTol, inverseMapping, calibrateMinMax)
{
}

// Overriding the parseInput method to control the actuator
ModuleState Schere::parseInput(int inputValue)
{
    if (inputValue == 0) // Close
    {
        _currentPositionIndex = 0;

        return positionMinimum(_storedPositions[0]);
    }
    else if (inputValue == 1) // Open
    {
        _currentPositionIndex = 1;

        return positionMaximum(_storedPositions[1]);
    }
    return ErrorState; // Invalid input
}

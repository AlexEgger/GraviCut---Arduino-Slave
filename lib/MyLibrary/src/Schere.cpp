#include "Schere.h"

// Constructor
Schere::Schere(int posPin, int negPin, int adcPin, uint16_t tol, const int positions[2], uint16_t closedLoopTol, bool inverseMapping)
    : BasePosServo(posPin, negPin, adcPin, tol, positions, closedLoopTol, inverseMapping) // Calling the base class constructor
{
    // No additional initialization needed
}

// Overriding the parseInput method to control the actuator
ModuleState Schere::parseInput(int inputValue)
{
    if (inputValue == 0) // Close
    {
        _currentPositionIndex = 0;
        Serial.println(F("Closing Schere"));
        
        return positionMinimum(_storedPositions[0]);
    }
    else if (inputValue == 1) // Open
    {
        _currentPositionIndex = 1;
        Serial.println(F("Opening Schere"));

        return positionMaximum(_storedPositions[1]);
    }
    return ErrorState; // Invalid input
}

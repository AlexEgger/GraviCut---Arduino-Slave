#include "Anschlag.h"

// Constructor
Anschlag::Anschlag(int posPin, int negPin, int adcPin, uint16_t tol, uint16_t closedLoopTol, bool inverseMapping)
    : BasePosServo(posPin, negPin, adcPin, tol, closedLoopTol, inverseMapping) // Calling the base class constructor
{
    // No additional initialization needed
}

// Constructor
Anschlag::Anschlag(int posPin, int negPin, int adcPin, uint16_t tol, const int positions[2], uint16_t closedLoopTol, bool inverseMapping)
    : BasePosServo(posPin, negPin, adcPin, tol, positions, closedLoopTol, inverseMapping) // Calling the base class constructor
{
    // No additional initialization needed
}

// Parse input commands from the master and trigger the corresponding function
ModuleState Anschlag::parseInput(int inputValue)
{
    // Serial.print("Received input: ");
    // Serial.println(inputValue);

    if (inputValue >= 200)
    {
        return positionTarget(inputValue); // Position to move to based on input
    }
    else
    {
        return positionMinimum(_storedPositions[0]); // Move to position for no collision
    }
}

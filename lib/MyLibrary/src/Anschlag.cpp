#include "Anschlag.h"

// Constructor
Anschlag::Anschlag(int posPin, int negPin, int adcPin, int tol)
    : BasePosServo(posPin, negPin, adcPin, tol) // Calling the base class constructor
{
    // No additional initialization needed
}

// Parse input commands from the master and trigger the corresponding function
ModuleState Anschlag::parseInput(int inputValue)
{
    Serial.print("Received input: ");
    Serial.println(inputValue);

    return positionTarget(inputValue); // Position to move to based on input
}

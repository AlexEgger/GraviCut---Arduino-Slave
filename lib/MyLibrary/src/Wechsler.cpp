#include "Wechsler.h"

/**
 * Constructor for Wechsler class
 * Initializes PWM pin and hard-coded parameters.
 * @param pwmPin: The pin used for PWM output.
 * @param rotationTimePerDegree: Time required for the motor to rotate 1 degree.
 * @param dutyCyclePWM: PWM duty cycle for motor control.
 */
Wechsler::Wechsler(int pwmPin, float rotationTimePerDegree, float dutyCyclePWM)
    : BaseActuator(), // Call the BaseActuator constructor
      _rotationTimePerDegree(rotationTimePerDegree),
      _dutyCyclePWM(dutyCyclePWM),
      _motorRunning(false),
      _elapsedTime(0),
      _pwmPin(pwmPin) // Initialize the PWM pin
{
    // Initialize PWM pin as output
    pinMode(_pwmPin, OUTPUT);
    // Ensure the motor is stopped initially
    digitalWrite(_pwmPin, LOW);
}

/**
 * Overriding the parseInput method to control the actuator.
 * This function should be called cyclically.
 * @param inputValue: The input value that controls the motor.
 *                    When inputValue == 1, the motor will rotate 72 degrees.
 * @return int: Returns 0 if the motor is still rotating,
 *              and 1 when the motor has finished its rotation.
 */
int Wechsler::parseInput(int inputValue)
{
    // If inputValue is 1, start the motor rotation
    if (inputValue == 1)
    {
        // Start the motor to rotate 72 degrees
        return rotateDegrees(72); // Call rotateDegrees and return its result
    }

    return 1; // Return 1 if no rotation is initiated (inputValue is not 1)
}

/**
 * Initiates the rotation of the motor by a specified number of degrees.
 * This function manages the motor's state and timing.
 * @param degrees: The number of degrees to rotate the motor.
 * @return uint8_t: Returns 0 if the motor is still rotating,
 *                  and 1 when the motor has finished its rotation.
 */
int Wechsler::rotateDegrees(int degrees)
{
    // If the motor is not currently running, initiate rotation
    if (!_motorRunning)
    {
        // Calculate the target time to rotate the specified number of degrees
        _targetTime = degrees * _rotationTimePerDegree; // Total time in seconds

        // Reset the elapsed time and mark the motor as running
        _elapsedTime = 0;
        _motorRunning = true;

        // Start the motor by setting the PWM duty cycle
        analogWrite(_pwmPin, _dutyCyclePWM * 255); // Convert to 0-255 scale for analogWrite

        // Record the start time
        _startTime = millis();
    }

    // If the motor is currently running, check the elapsed time
    if (_motorRunning)
    {
        // Get the elapsed time in seconds
        _elapsedTime = (millis() - _startTime) / 1000.0;

        // If the elapsed time has exceeded the target time
        if (_elapsedTime >= _targetTime)
        {
            // Stop the motor by setting the PWM duty cycle to 0
            analogWrite(_pwmPin, 0);

            // Mark the motor as stopped
            _motorRunning = false;

            return 1; // Motor rotation is complete, return 1
        }
        else
        {
            return 0; // Motor is still rotating, return 0
        }
    }

    return 1; // If no rotation was initiated or if the motor has stopped, return 1
}

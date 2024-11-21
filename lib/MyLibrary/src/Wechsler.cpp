#include <Servo.h>
#include "Wechsler.h"

/**
 * Constructor for Wechsler class
 * Initializes PWM pin and hard-coded parameters.
 * @param pwmPin: The pin used for PWM output.
 * @param rotationTimePerDegree: Time required for the motor to rotate 1 degree.
 */
Wechsler::Wechsler(int pwmPin, float rotationTimePerDegree)
    : BaseActuator(), // Call the BaseActuator constructor
      _rotationTimePerDegree(rotationTimePerDegree),
      _motorRunning(false),
      _elapsedTime(0),
      _pin(pwmPin) // Initialize the PWM pin
{
    // Attach the servo to the PWM pin
    _servo.attach(_pin);
    // Ensure the motor is stopped initially
    _servo.write(90); // Assuming 90 is the position to stop the servo
}

/**
 * Overriding the parseInput method to control the actuator.
 * This function should be called cyclically.
 * @param inputValue: The input value that controls the motor.
 *                    When inputValue == 1, the motor will rotate 72 degrees.
 * @return ModuleState: Returns 0 if the motor is still rotating,
 *                      and 1 when the motor has finished its rotation.
 */
ModuleState Wechsler::parseInput(int inputValue)
{
    // If inputValue is 1, start the motor rotation
    if (inputValue == 1)
    {
        // Start the motor to rotate 72 degrees
        return rotateDegrees(72); // Call rotateDegrees and return its result
    }

    return CompletedState; // Return 1 if no rotation is initiated (inputValue is not 1)
}

/**
 * Initiates the rotation of the motor by a specified number of degrees.
 * This function manages the motor's state and timing.
 * @param degrees: The number of degrees to rotate the motor.
 * @return ModuleState: Returns 0 if the motor is still rotating,
 *                      and 1 when the motor has finished its rotation.
 */
ModuleState Wechsler::rotateDegrees(int degrees)
{
    // If the motor is not currently running, initiate rotation
    if (!_motorRunning)
    {
        // Calculate the target time to rotate the specified number of degrees
        _targetTime = degrees * _rotationTimePerDegree; // Total time in seconds

        // Reset the elapsed time and mark the motor as running
        _elapsedTime = 0;
        _motorRunning = true;

        // Start the motor by setting the servo to 180 (turn on)
        _servo.write(120);

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
            // Stop the motor by setting the servo to 90 (turn off)
            _servo.write(90);

            // Mark the motor as stopped
            _motorRunning = false;

            return CompletedState; // Motor rotation is complete, return CompletedState
        }
        else
        {
            return RunningState; // Motor is still rotating, return RunningState
        }
    }

    return CompletedState; // If no rotation was initiated or if the motor has stopped, return CompletedState
}

/**
 * Initiates the rotation of the motor for a specified duration.
 * This function manages the motor's state and timing.
 * @param duration: The duration (in milliseconds) to rotate the motor.
 * @return ModuleState: Returns 0 if the motor is still rotating,
 *                      and 1 when the motor has finished its rotation.
 */
ModuleState Wechsler::rotateForDuration(unsigned long duration)
{
   // Serial.println("Rotating the motor for a specified duration...");
    // If the motor is not currently running, initiate rotation
    if (!_motorRunning)
    {
       // Serial.println("Starting motor rotation...");
        // Reset the elapsed time and mark the motor as running
        _elapsedTime = 0;
        _motorRunning = true;

       // Serial.println("Setting servo to ON position...");
        // Start the motor by setting the servo to 180 (turn on)
        _servo.write(120);

        // Record the start time
        _startTime = millis();

        // Set the target time for the rotation
        _targetTime = duration; // Target time in milliseconds
    }

    // If the motor is currently running, check the elapsed time
    if (_motorRunning)
    {
       // Serial.println("Checking elapsed time...");
        // Get the elapsed time in milliseconds
        _elapsedTime = millis() - _startTime;

       // Serial.println("Elapsed time: " + String(_elapsedTime));

        // If the elapsed time has exceeded the target time
        if (_elapsedTime >= _targetTime)
        {
            // Stop the motor by setting the servo to 90 (turn off)
            _servo.write(90);

            // Mark the motor as stopped
            _motorRunning = false;

            return CompletedState; // Motor rotation is complete, return CompletedState
        }
        else
        {
            return RunningState; // Motor is still rotating, return RunningState
        }
    }

    return CompletedState; // If no rotation was initiated or if the motor has stopped, return CompletedState
}

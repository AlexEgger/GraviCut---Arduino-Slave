#ifndef BASEPOSSERVO_H
#define BASEPOSSERVO_H

#include "BaseActuator.h"

// Abgeleitete Klasse BasePosServo
class BasePosServo : public BaseActuator
{
protected:
    enum motorDirection
    {
        Off = 0,
        Positive,
        Negative
    };

    int _posPin;                      // Positive terminal pin
    int _negPin;                      // Negative terminal pin
    int _adcPin;                      // ADC input pin for position feedback
    int _position;                    // Current position in 1/100 cm
    int _adcValue;                    // Current ADC value
    int _storedPositions[2];          // Fixed positions the motor should move to (optional)
    const int _tol;                   // Tolerance in 1/100 cm
    motorDirection _currentDirection; // Direction the motor is moving in

    // Convert from ADC value to a decimal value
    virtual uint16_t mapADC(int x, int in_min, int in_max, uint16_t out_min, uint16_t out_max);

    // Methode zur Aktualisierung der aktuellen Position des Servos
    virtual void updatePosition();

    // Methode zum Ansteuern der Ausgänge
    virtual void goDirection(motorDirection direction);

    // Move actuator to a specific position
    virtual ModuleState positionTarget(int targetPosition);

    // Move actuator to at least targetPosition
    virtual ModuleState positionMinimum(int targetPosition);

    // Move actuator to at maximum targetPosition
    virtual ModuleState positionMaximum(int targetPosition);

public:
    // Standard-Konstruktor
    BasePosServo(int posPin, int negPin, int adcPin, int tol);

    // Überladener Konstruktor mit zusätzlichem Positionsarray
    BasePosServo(int posPin, int negPin, int adcPin, int tol, const int positions[2]);
};

#endif // BASEPOSSERVO_H

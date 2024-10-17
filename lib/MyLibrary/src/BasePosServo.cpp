#include "BasePosServo.h"
#include <Arduino.h>
#include "myEnums.h"

// Konstruktor der abgeleiteten Klasse BasePosServo
BasePosServo::BasePosServo(int posPin, int negPin, int adcPin, int tol)
    : _posPin(posPin), _negPin(negPin), _adcPin(adcPin), _position(0), _tol(tol), _currentDirection(Off)
{
    // Initialisierung der Pins als Ein- und Ausgänge
    pinMode(_posPin, OUTPUT);
    pinMode(_negPin, OUTPUT);
    pinMode(_adcPin, INPUT);

    // Motorpins ausschalten
    digitalWrite(_posPin, LOW);
    digitalWrite(_negPin, LOW);

    updatePosition();
}

// Überladener Konstruktor der abgeleiteten Klasse BasePosServo
BasePosServo::BasePosServo(int posPin, int negPin, int adcPin, int tol, const int positions[2])
    : BasePosServo(posPin, negPin, adcPin, tol) // Aufruf des Standardkonstruktors
{
    // Speichern der übergebenen Positionen im Array
    _storedPositions[0] = positions[0];
    _storedPositions[1] = positions[1];
}

// Methode zur Umwandlung des ADC-Werts in einen Dezimalwert
uint16_t BasePosServo::mapADC(int x, int in_min, int in_max, uint16_t out_min, uint16_t out_max)
{
    // Sicherstellen, dass der Wert von x innerhalb der erwarteten Grenzen liegt
    if (x < in_min)
        x = in_min;
    if (x > in_max)
        x = in_max;

    // Invertiertes Mapping: Maximaler Eingang führt zum minimalen Ausgang und umgekehrt
    return static_cast<uint16_t>((static_cast<float>(in_max - x) * (out_max - out_min) / (in_max - in_min)) + out_min);
}

// Methode zur Aktualisierung der aktuellen Position des Servos
void BasePosServo::updatePosition()
{
    _adcValue = analogRead(_adcPin);             // Read ADC value (range: 0 to 1023)
    _position = mapADC(_adcValue, 0, 1023, 10, 520); // Map from 0-1023 to 10-520 (Position in 1/100 cm)

    Serial.print(F("ADC-Wert: "));
    Serial.println(_adcValue);
    Serial.print(F("Position: "));
    Serial.println(_position);
}

// Methode zum Ansteuern der Ausgänge
void BasePosServo::goDirection(motorDirection direction)
{
    // Bei Änderung der Richtung
    if (_currentDirection != direction)
    {
        _currentDirection = direction;
        Serial.print("Direction changed to: ");

        switch (direction)
        {
        case Positive:
            digitalWrite(_negPin, LOW);
            digitalWrite(_posPin, HIGH);
            Serial.println("Positive Direction");
            break;
        case Negative:
            digitalWrite(_posPin, LOW);
            digitalWrite(_negPin, HIGH);
            Serial.println("Negative Direction");
            break;
        default:
            digitalWrite(_posPin, LOW);
            Serial.println("Off");
            break;
        }
    }
}

// Move actuator to a specific position
ModuleState BasePosServo::positionTarget(int targetPosition)
{
    updatePosition(); // Position aktualisieren
    Serial.print("Moving to position: ");
    Serial.println(targetPosition);
    if (_position < targetPosition - _tol)
    {
        goDirection(Positive);

        Serial.println("Increasing position");

        return RunningState; // Still running
    }
    else if (_position > targetPosition + _tol)
    {
        goDirection(Negative);

        Serial.println("Decreasing position");

        return RunningState; // Still running
    }
    else
    {
        goDirection(Off);

        Serial.println("Position reached");

        return CompletedState; // Operation completed
    }
}

// Increase actuator position to at least targetPosition
ModuleState BasePosServo::positionMinimum(int targetPosition)
{
    updatePosition();
    if (_position < targetPosition) // Compare with the defined open position
    {
        goDirection(Positive);
        return RunningState; // Still running
    }
    else
    {
        goDirection(Off);
        return CompletedState; // Operation completed
    }
}

// Reduce actuator position to at maximum targetPosition
ModuleState BasePosServo::positionMaximum(int targetPosition)
{
    updatePosition();
    if (_position > targetPosition) // Compare with the defined closed position
    {
        goDirection(Negative);
        return RunningState; // Still running
    }
    else
    {
        goDirection(Off);
        return CompletedState; // Operation completed
    }
}
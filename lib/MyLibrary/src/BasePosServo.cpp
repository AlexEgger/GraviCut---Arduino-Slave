#include "BasePosServo.h"
#include <Arduino.h>

// Konstruktor der abgeleiteten Klasse BasePosServo
BasePosServo::BasePosServo(int posPin, int negPin, int adcPin, int tol)
    : _posPin(posPin), _negPin(negPin), _adcPin(adcPin), _position(0), _tol(tol)
{
    // Initialisierung der Pins als Ein- und Ausgänge
    pinMode(_posPin, OUTPUT);
    pinMode(_negPin, OUTPUT);
    pinMode(_adcPin, INPUT);
}

// Methode zur Umwandlung des ADC-Werts in einen Dezimalwert
uint16_t BasePosServo::mapADC(float x, float in_min, float in_max, uint16_t out_min, uint16_t out_max)
{
    // Standard-Arduino-Mapping-Funktion zur Umwandlung des ADC-Werts
    return static_cast<uint16_t>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

// Methode zur Aktualisierung der aktuellen Position des Servos
void BasePosServo::updatePosition()
{
    int adcValue = analogRead(_adcPin);             // Read ADC value (range: 0 to 1023)
    _position = mapADC(adcValue, 0, 1023, 10, 520); // Map from 0-1023 to 10-520 (Position in 1/100 cm)
}

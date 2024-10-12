#ifndef BASEPOSSERVO_H
#define BASEPOSSERVO_H

#include "BaseActuator.h"

// Abgeleitete Klasse BasePosServo
class BasePosServo : public BaseActuator
{
protected:
    int _posPin;       // Positive terminal pin
    int _negPin;       // Negative terminal pin
    int _adcPin;       // ADC input pin for position feedback
    int _position; // Current position in 1/100 cm
    const int _tol; // Tolerance in 1/100 cm

    // Convert from ADC value to a decimal value
    virtual uint16_t mapADC(float x, float in_min, float in_max, uint16_t out_min, uint16_t out_max);

    // Methode zur Aktualisierung der aktuellen Position des Servos
    virtual void updatePosition();

public:
    // Konstruktor der abgeleiteten Klasse BasePosServo
    BasePosServo(int posPin, int negPin, int adcPin, int tol);
};

#endif // BASEPOSSERVO_H

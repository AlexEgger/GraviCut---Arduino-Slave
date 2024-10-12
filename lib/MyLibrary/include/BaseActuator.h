#ifndef BASEACTUATOR_H
#define BASEACTUATOR_H

#include <Arduino.h>

// Basisklasse Actuator
class BaseActuator
{
public:
    // Konstruktor der Basisklasse Actuator
    BaseActuator() {}

    // Virtuelle Methode parseInput() zur Verarbeitung von Eingabewerten
    // Diese Methode muss in abgeleiteten Klassen überschrieben werden
    virtual int parseInput(int inputValue) = 0;
};

#endif // BASEACTUATOR_H

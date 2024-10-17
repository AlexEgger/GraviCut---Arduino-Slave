#ifndef WECHSLER_H
#define WECHSLER_H

#include <Arduino.h>
#include "BaseActuator.h"

class Wechsler : public BaseActuator
{
private:
    // Zeit, die der Motor benötigt, um 1 Grad zu drehen (in Millisekunden)
    float _rotationTimePerDegree;
    float _dutyCyclePWM;      // PWM-Duty-Cycle zur Steuerung des Motors
    float _targetTime;        // Zielzeit in Sekunden, um die Drehung abzuschließen
    float _elapsedTime;       // Verstrichene Zeit seit dem Start der Motorrotation
    bool _motorRunning;       // Flag, das angibt, ob der Motor gerade läuft
    unsigned long _startTime; // Startzeit der Rotation in Millisekunden
    int _pwmPin;              // PWM-Pin für die Motorsteuerung

    // Initiates the rotation of the motor by a specified number of degrees
    ModuleState rotateDegrees(int degrees);

public:
    // Konstruktor
    Wechsler(int pwmPin, float rotationTimePerDegree, float dutyCyclePWM);

    // Überschreiben der parseInput-Methode zur Steuerung des Aktuators
    ModuleState parseInput(int inputValue) override;
};

#endif // WECHSLER_H

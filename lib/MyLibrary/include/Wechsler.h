#ifndef WECHSLER_H
#define WECHSLER_H

#include <Arduino.h>
#include <Servo.h>
#include "BaseActuator.h"

class Wechsler : public BaseActuator
{
private:
    // Zeit, die der Motor benötigt, um 1 Grad zu drehen (in Millisekunden)
    Servo _servo;                 // Servo-Objekt zur Motorsteuerung
    float _rotationTimePerDegree; // Zeit, die der Motor benötigt, um 1 Grad zu drehen (in Sekunden)
    float _targetTime;            // Zielzeit in Sekunden, um die Drehung abzuschließen
    float _elapsedTime;           // Verstrichene Zeit seit dem Start der Motorrotation
    bool _motorRunning;           // Flag, das angibt, ob der Motor gerade läuft
    unsigned long _startTime;     // Startzeit der Rotation in Millisekunden
    int _pin;                     // PWM-Pin für die Motorsteuerung

    // Initiates the rotation of the motor by a specified number of degrees
    ModuleState rotateDegrees(int degrees);

public:
    // Konstruktor
    Wechsler(int pwmPin, float rotationTimePerDegree);

    // Überschreiben der parseInput-Methode zur Steuerung des Aktuators
    ModuleState parseInput(int inputValue) override;

    // Initiates the rotation of the motor for a specified duration
    ModuleState rotateForDuration(unsigned long duration);
};

#endif // WECHSLER_H

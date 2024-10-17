#ifndef MAGAZIN_H
#define MAGAZIN_H

#include <Arduino.h> // Include Arduino header for pin and timing handling
#include "BaseActuator.h"

// Struktur für einen Solenoid-Motor und einen zugehörigen Sensor
struct SolenoidSensorPair
{
    int solenoidPin; // Pin zur Steuerung des Solenoids
    int sensorPin;   // Pin zum Lesen des Sensors

    // Standardkonstruktor
    SolenoidSensorPair() : solenoidPin(0), sensorPin(0) {} // Initialisiere mit Standardwerten

    // Benutzerdefinierter Konstruktor
    SolenoidSensorPair(int solPin, int senPin)
        : solenoidPin(solPin), sensorPin(senPin) {}
};

class Magazin : public BaseActuator
{
private:
    SolenoidSensorPair _pairs[3];      // Array von Solenoid-Sensor-Paaren
    unsigned long _activateDuration;   // Dauer, für die der Solenoid aktiv bleibt (in ms)
    unsigned long _deactivateDuration; // Dauer, bevor die Funktion 1 zurückgibt (in ms)
    unsigned long _timer;              // Timer zur Messung der Zeit
    bool _isActivated;                 // Flag, ob der Solenoid aktiviert wurde

    // Interne Logik für das Ansteuern und das Auslesen des Sensors
    ModuleState activateControl(int index);

public:
    // Konstruktor: Initialisiert die Solenoids, Sensoren und Zeiten
    Magazin(int solPins[3], int senPins[3], unsigned long activateDuration, unsigned long deactivateDuration);

    // Überschreiben der Methode parseInput aus der Basisklasse
    ModuleState parseInput(int inputValue) override;
};

#endif // MAGAZIN_H

#include "Magazin.h"

// Konstruktor: Initialisiert die Solenoids, Sensoren und Zeiten
Magazin::Magazin(int solPins[3], int senPins[3], unsigned long activateDuration, unsigned long deactivateDuration)
    : _activateDuration(activateDuration),
      _deactivateDuration(deactivateDuration),
      _isActivated(false)
{
    // Initialisierung der Solenoid-Sensor-Paare
    for (int i = 0; i < 3; ++i)
    {
        _pairs[i] = SolenoidSensorPair(solPins[i], senPins[i]);
        pinMode(_pairs[i].solenoidPin, OUTPUT); // Setze Solenoid-Pin als Ausgang
        pinMode(_pairs[i].sensorPin, INPUT);    // Setze Sensor-Pin als Eingang
    }

    _timer = millis(); // Initialisiere den Timer
}

// Interne Logik zur Steuerung der Solenoids und zum Auslesen der Sensoren
int Magazin::activateControl(int index)
{
    if (!_isActivated) // Wenn der Solenoid noch nicht aktiviert ist
    {
        digitalWrite(_pairs[index].solenoidPin, HIGH); // Solenoid einschalten
        _timer = millis();                             // Timer zurücksetzen
        _isActivated = true;                           // Aktivierungsstatus setzen
        return 0;                                      // Noch nicht fertig, Zeit läuft
    }

    // Solenoid ist aktiviert, Zeit prüfen
    unsigned long elapsedTime = millis() - _timer;
    if (elapsedTime >= _activateDuration && elapsedTime < _activateDuration + _deactivateDuration)
    {
        digitalWrite(_pairs[index].solenoidPin, LOW); // Solenoid ausschalten
    }

    // Prüfen, ob die gesamte Zeit (Aktivierungs- und Deaktivierungsdauer) abgelaufen ist
    if (elapsedTime >= _activateDuration + _deactivateDuration)
    {
        // Sensorwert lesen
        int sensorValue = digitalRead(_pairs[index].sensorPin);

        _isActivated = false; // Aktivierungsstatus zurücksetzen

        // Wenn der Sensorwert erkannt wurde, Rückgabewert 2
        if (sensorValue == HIGH)
        {
            return 2; // Sensorwert erkannt
        }
        return 1; // Vorgang abgeschlossen, aber Sensor nicht ausgelöst
    }

    return 0; // Vorgang läuft noch
}

// Verarbeitung der Eingabe, ruft activateControl basierend auf inputValue auf
int Magazin::parseInput(int inputValue)
{
    int motorIndex = inputValue - 1; // Index des Solenoids (0-2)
    if (motorIndex >= 0 && motorIndex < 3)
    {
        return activateControl(motorIndex); // Rufe activateControl mit dem entsprechenden Index auf
    }
    return -1; // Ungültige Eingabe
}

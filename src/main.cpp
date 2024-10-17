#include <Wire.h>    // Arduino I2C-Bibliothek einbinden
#include <Arduino.h> // Arduino Bibliothek einbinden

#include "Magazin.h"  // Magazin-Klasse einbinden
#include "Anschlag.h" // Anschlag-Klasse einbinden
#include "Schieber.h" // Schieber-Klasse einbinden
#include "Schere.h"   // Schere-Klasse einbinden
#include "Wechsler.h" // Wechsler-Klasse einbinden

#include "MyEnums.h" // Enumerationsdatei einbinden

/* Daten, die angepasst werden müssen */
const bool testI2C = false;                  // Testmodus für I2C-Kommunikation
const uint8_t slaveAddress = BoardFuerAlles; // I2C-Adresse des Slaves
const uint8_t msgSize = 2;                   // Größe der Bestätigungsnachricht, muss mit Master abgestimmt sein
// Erste Stelle: Antwortwert, Zweite Stelle: Index des leeren Magazins

int testValue = 0; // Testwert für die Aktoren

// Kontrollklasse für Aktoren
BaseActuator *modules[NumberOfModules]; // Array von Aktoren

/* Daten, die nicht angepasst werden müssen */
int16_t requestValues[NumberOfModules];    // Werte, die vom Master an die Slaves ausgegeben werden
ModuleState moduleStates[NumberOfModules]; // Werte, die von den Slaves an den Master zurückgegeben werden
const uint8_t buffSize = 4;                // Größe des Buffers für empfangene Daten
char buff[buffSize] = {0};                 // Buffer für empfangene Daten, initialisiert mit 0
char msg[msgSize + 1] = {0};               // Bestätigungsnachricht
Response response = Completed;             // Antwortwert
int8_t emptyMagazine = 0;                  // Index des leeren Magazins
uint16_t readValue = 0;                    // Gelesener Wert

void receiveFromMaster(int numBytes); // Funktion zum Empfangen der Eingabe

void parseInput(); // Funktion zum Verarbeiten der Eingabe

void respondToMaster(); // Funktion zum Antworten auf den Master

void getResponses(); // Funktion zum Überprüfen der Antworten

void int2Char(int numberToConvert, char *output); // Funktion zum Konvertieren von Integer in Char-Array

void charArray2IntArray(const char *charArray, int8_t *output); // Funktion zum Konvertieren von Char-Array in Integer-Array

void char2Int(char *charArray, uint16_t &output); // Funktion zum Konvertieren von Char-Array in Integer

void testFunc(); // Funktion zum Testen der Aktoren

uint16_t mapADC(float x, float in_min, float in_max, uint16_t out_min, uint16_t out_max); // Funktion zum Umwandeln des ADC-Werts in einen Dezimalwert

// Setup-Funktion für Initialisierungen
void setup()
{
  // Baudrate für die serielle Kommunikation auf 9600 setzen
  Serial.begin(9600);

  // I2C-Adresse des Slaves setzen
  Wire.begin(slaveAddress);

  // Funktion für den Empfang von Daten vom Master setzen
  Wire.onReceive(receiveFromMaster);

  // Funktion für die Antwort an den Master setzen
  Wire.onRequest(respondToMaster);

  // Meldung ausgeben, dass der I2C-Slave initialisiert ist
  Serial.println(F("I2C-Slave initialisiert"));
  Serial.print(F("I2C-Adresse: "));
  Serial.println(slaveAddress);

  /* Initialisieren der Aktoren*/
  // Magazin Daten
  int solenoidPins[3] = {13, 11, 9}; // C-Array für Solenoid-Pins
  int sensorPins[3] = {12, 10, 8};   // C-Array für Sensor-Pins
  int timeOn = 1000;                 // Aktivierungszeit für das Magazin
  int timeOff = 1000;                // Deaktivierungszeit für das Magazin

  modules[MagazinModule] = new Magazin(solenoidPins, sensorPins, timeOn, timeOff); // Magazin (Solenoid-Pins, Sensor-Pins, Aktivierungs- und Deaktivierungsdauer)

  // Schieber Daten
  int posPin = 5;                        // Positiver Pin für den Schieber
  int negPin = 4;                        // Negativer Pin für den Schieber
  int adcPin = 15;                       // ADC-Pin für den Schieber
  int tol = 5;                           // Toleranz für den Schieber
  int positionenSchieber[2] = {30, 500}; // Positionen für Schieber

  modules[SchieberModule] = new Schieber(posPin, negPin, adcPin, tol, positionenSchieber); // Schieber (Positiver Pin, Negativer Pin, ADC-Pin, Toleranz, Position A, Position B)

  // Schere Daten
  posPin = 7;                          // Positiver Pin für die Schere
  negPin = 6;                          // Negativer Pin für die Schere
  adcPin = 14;                         // ADC-Pin für die Schere
  tol = 5;                             // Toleranz für die Schere
  int positionenSchere[2] = {30, 500}; // Positionen für Schere

  modules[SchereModule] = new Schere(posPin, negPin, adcPin, tol, positionenSchere); // Schere (Positiver Pin, Negativer Pin, ADC-Pin, Toleranz, Position für das Öffnen, Position für das Schließen)

  // Anschlag Daten
  posPin = 2;  // Positiver Pin für den Anschlag
  negPin = 17; // Negativer Pin für den Anschlag, 17 == A3
  adcPin = 16; // ADC-Pin für den Anschlag
  tol = 3;    // Toleranz für den Anschlag

  modules[AnschlagModule] = new Anschlag(posPin, negPin, adcPin, tol); // Anschlag (Positiver Pin, Negativer Pin, ADC-Pin, Toleranz)

  // Wechsler Daten
  int pwmPin = 3;                       // PWM-Pin für den Wechsler
  float rotationTimePerDegree = 0.0035; // Drehzeit pro Grad für den Wechsler
  float dutyCyclePWM = 0.75f;           // PWM-Duty-Cycle für den Wechsler

  modules[WechslerModule] = new Wechsler(pwmPin, rotationTimePerDegree, dutyCyclePWM); // Wechsler (PWM-Pin, Drehzeit pro Grad, PWM-Duty-Cycle)

  // Standard Status für die Module setzen
  for (uint8_t i = 0; i < NumberOfModules; i++)
  {
    requestValues[i] = -1;
    moduleStates[i] = CompletedState;
  }
}

// Hauptprogramm
void loop()
{
  // testFunc();     // Testfunktion
  getResponses(); // Überprüfen der Antworten

  delay(100);
}

void receiveFromMaster(int numBytes)
{
  int index = 0; // Index zum Speichern der Daten im Buffer
  while (Wire.available() && index < buffSize)
  {                                  // Solange Daten verfügbar sind und Buffer nicht voll ist
    char incomingData = Wire.read(); // Gelesenen Wert speichern
    // Serial.print(F("Empfangen: "));
    // Serial.println(incomingData);
    buff[index++] = incomingData; // Speichere den empfangenen Wert im Buffer
  }
  buff[index] = '\0'; // Nullterminierung

  // Serial.println(F("Empfangen abgeschlossen"));
  // Serial.print(F("Empfangene Daten: "));
  // Serial.println(buff);

  parseInput(); // Empfangene Daten verarbeiten
}

void parseInput()
{
  // Konvertiere die empfangenen Daten in einen Integer-Wert
  readValue = 0; // Vom Master gelesener Wert als Integer
  char2Int(buff, readValue);
  buff[0] = '\0'; // Buffer zurücksetzen

  // Abhängig vom empfangenen Status reagieren
  // --> zB input = 10 --> currentModule = 1, requestValues[1] = 0;
  // --> zB input = 5200 --> currentModule = 5, requestValues[5] = 200;
  for (uint16_t i = 10; i < 10000; i *= 10) // Schleife durch die Zehnerpotenzen
  {
    if (readValue < i * 10) // Wenn der Wert kleiner als die nächste Zehnerpotenz ist
    {
      Module currentModule = static_cast<Module>(readValue / i); // Aktueller Funktionsblock
      requestValues[currentModule] = readValue % i;              // Wert für den Funktionsblock speichern
      moduleStates[currentModule] = RunningState;                // Status auf "Running" setzen
      response = Running;                                        // Antwortwert auf "Running" setzen

      // Serial.print(F("Index: "));
      // Serial.println(i);
      // Serial.print(F("Funktionsblock: "));
      // Serial.println(currentModule);
      // Serial.print(F("Wert: "));
      // Serial.println(requestValues[currentModule]);

      break;
    }
  }
}

void respondToMaster()
{
  // Antwort an den Master vorbereiten
  int responseValue = 10 * response + emptyMagazine; // Antwortwert vorbereiten
  int2Char(responseValue, msg);                      // Antwortwert in ein char-Array konvertieren
  msg[msgSize] = '\0';                               // Nullterminierung
  Serial.print(F("Antwort wird gesendet: "));        // Meldung ausgeben
  Serial.println(msg);                               // Antwort ausgeben

  Wire.write(msg, msgSize); // Antwort an den Master senden
  // Serial.println(F("Antwort gesendet")); // Meldung ausgeben
}

// Überprüft die Antworten
void getResponses()
{
  // Serial.println(F("Antworten werden überprüft")); // Meldung ausgeben
  // Serial.print(F("Antwortwert: "));                // Meldung ausgeben
  // Serial.println(response);                        // Antwortwert ausgeben

  if (response == Running) // Wenn noch nicht alle Befehle fertig verarbeitet wurden
  {
    Response tempResponse = Completed; // Antwortwert auf 1 setzen
    /* Abarbeiten der empfangenen Befehle*/
    for (uint8_t responder = 0; responder < NumberOfModules; responder++) // Für jeden Funktionsblock
    {
      if (requestValues[responder] != -1) // Wenn der Wert gültig ist
      {
        if (moduleStates[responder] == RunningState) // Wenn der Vorgang noch nicht abgeschlossen ist
        {
          if (testI2C) // Wenn der Testmodus aktiviert ist
          {
            moduleStates[responder] = CompletedState; // Testantwort setzen
          }
          else // Wenn der Testmodus nicht aktiviert ist
          {
            // Serial.print("Funktionsblock: ");
            // Serial.println(responder);
            // Serial.print("Wert: ");
            // Serial.println(requestValues[responder]);

            moduleStates[responder] = modules[responder]->parseInput(requestValues[responder]); // Funktion ausführen

            if (responder == MagazinModule) // Magazin Füllstand kontrollieren                                                                               // Wenn ein Fehler aufgetreten ist
            {
              if (moduleStates[responder] == ErrorState)  // Wenn ein Fehler aufgetreten ist
                emptyMagazine = requestValues[responder]; // Leeres Magazin
            }
            else
            {
              emptyMagazine = 0; // Kein leeres Magazin
            }
          }

          // Überprüfe den Status des Auftrags
          if (moduleStates[responder] != RunningState) // Wenn der Vorgang abgeschlossen ist
          {
            requestValues[responder] = -1; // Ungültigen Wert setzen
          }
          else // Wenn der Vorgang noch nicht abgeschlossen ist
          {
            tempResponse = Running; // Antwortwert auf 0 setzen
          }
        }
      }
    }
    response = tempResponse; // Antwortwert setzen
  }
}

// Konvertiert einen int-Wert in ein char-Array
void int2Char(int numberToConvert, char *output) // Konvertiert einen int-Wert in ein char-Array
{
  uint8_t length = buffSize; // Länge des Buffers
  // int in char-Array umwandeln
  snprintf(output, length, "%d", numberToConvert); // snprintf vermeidet Bufferüberlauf
}

// Konvertiert ein char-Array in ein int-Array
void charArray2IntArray(const char *charArray, int8_t *output) // Konvertiert ein char-Array in ein int-Array
{
  size_t length = strlen(charArray); // Länge des char-Arrays bestimmen

  for (size_t i = 0; i < length; ++i)
  {
    // Sicherstellen, dass das char-Element eine Ziffer ist
    if (charArray[i] >= '0' && charArray[i] <= '9')
    {
      output[i] = charArray[i] - '0'; // Konvertieren von char zu int
    }
    else
    {
      output[i] = -1; // Ungültige Eingaben als -1 kennzeichnen
    }
  }
}

// Konvertiert ein mehrstelliges char-Array in einen Integer-Wert
void char2Int(char *charArray, uint16_t &output) // Konvertiert ein mehrstelliges char-Array in einen Integer-Wert
{
  output = 0;                        // Setze den Ausgangswert auf 0
  size_t length = strlen(charArray); // Länge des char-Arrays bestimmen

  // Schleife durch jedes Zeichen im char-Array
  for (size_t i = 0; i < length; ++i)
  {
    // Sicherstellen, dass das char-Element eine Ziffer ist
    if (charArray[i] >= '0' && charArray[i] <= '9')
    {
      // Verschiebe den bisherigen Wert um eine Stelle nach links und addiere die neue Ziffer
      output = output * 10 + (charArray[i] - '0');
    }
  }
}

// // Methode zur Umwandlung des ADC-Werts in einen Dezimalwert
// uint16_t mapADC(float x, float in_min, float in_max, uint16_t out_min, uint16_t out_max)
// {
//   // Standard-Arduino-Mapping-Funktion zur Umwandlung des ADC-Werts
//   return static_cast<uint16_t>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
// }
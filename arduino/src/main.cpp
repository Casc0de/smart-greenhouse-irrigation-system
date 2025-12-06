#include <Arduino.h>
#include <Wire.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ControladorComunicacionConRaspberry.h"
#include "ControladorTiempo.h"

// Objetos globales
ControladorRiego ctlRiego;
ControladorComunicacionConRaspberry ctlComRaspberry(Serial);
ControladorTiempo ctlTiempo;

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  ctlRiego.begin();
  ctlTiempo.begin();

  Serial.println("=== Sistema de riego automático iniciado ===");
}

void loop()
{
  // 1) Preguntar al ControladorTiempo si es momento de riego según los horarios
  if (ctlTiempo.esMomentoDeRiego())
  {
    Serial.println(">> ControladorTiempo: es momento de riego según horario");
    ctlRiego.iniciarCiclo(); // Arranca la máquina de estados de riego
  }

  // 2) Avanzar SIEMPRE la máquina de estados de riego
  ctlRiego.regar();

  // 3) (Opcional) Debug de hora cada cierto tiempo
  static unsigned long ultimoPrint = 0;
  unsigned long ahora = millis();

  if (ahora - ultimoPrint >= 5000UL)
  {
    ultimoPrint = ahora;

    DateTime now = ctlTiempo.rtc.now();
    Serial.print("RTC -> DiaOfWeek: ");
    Serial.print(now.dayOfTheWeek()); // 0..6
    Serial.print(" | Hora: ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.println(now.minute());
  }

  // Pequeño delay para no saturar CPU/Serial
  delay(50);
}

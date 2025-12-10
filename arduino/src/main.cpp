#include <Arduino.h>
#include <Wire.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ControladorComunicacionConRaspberry.h"
#include "ControladorTiempo.h"
#include "Bomba.h"

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
  ctlRiego.bomba.encender();
  // 1) Preguntar al ControladorTiempo si es momento de riego según los horarios
}

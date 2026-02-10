#include <Arduino.h>
#include <Wire.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ControladorComunicacionConRaspberry.h"
#include "ControladorTiempo.h"
#include "Bomba.h"

// put function and object declarations here:

ControladorComunicacionConRaspberry ctlComRaspberry(Serial);
ControladorRiego ctlRiego(ctlComRaspberry);
ControladorTiempo ctlTiempo;

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  ctlRiego.begin();
  ctlTiempo.begin();

  // Serial.println("=== Sistema de riego automático iniciado ===");
}

void loop()
{
  if (ctlTiempo.esMomentoDeRiego())
  {
    // Serial.println(">> ControladorTiempo: es momento de riego según horario");
    ctlRiego.iniciarCiclo();
  }

  ctlRiego.regar();
}

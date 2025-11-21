#include <Arduino.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ControladorComunicacionConRaspberry.h"
#include "ControladorTiempo.h"

// put function and object declarations here:
ControladorRiego ctlRiego;
ControladorComunicacionConRaspberry ctlComRaspberry(Serial);
ControladorTiempo ctlTiempo;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  ctlRiego.begin();
  ctlTiempo.begin();
}

void loop()
{
  // ctlRiego.regar();
  ctlRiego.ctlSensores.tanques[0].nivel = 0; // Simular un nivel de tanque
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[0]);
  delay(2000);
  ctlRiego.ctlSensores.tanques[0].nivel = 50; // Simular un nivel de tanque
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[0]);
  delay(2000);
  // ctlComRaspberry.enviarManometro(ctlRiego.ctlSensores.manometroSensor);
  // delay(2000);
  ctlRiego.ctlSensores.tanques[0].nivel = 100; // Simular un nivel de tanque
  delay(2000);
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[0]);
  delay(2000);
}

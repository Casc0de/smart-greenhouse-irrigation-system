#include <Arduino.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ServicioComunicacion.h"
#include "ControladorTiempo.h"

// function and object declarations here:
ServicioComunicacion srvComm(Serial);
ControladorRiego ctlRiego(srvComm);
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
}

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

const unsigned long DURACION = 10000; // 10 segundos

static bool activo = false;
static unsigned long tInicio = 0;

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
  // ctlRiego.regar();
  ctlRiego.ctlSensores.manometroSensor.presion = 25.5; // Simular una presión
  ctlComRaspberry.enviarManometro(ctlRiego.ctlSensores.manometroSensor);
  delay(2000);

  ctlRiego.ctlSensores.manometroSensor.presion = 20; // Simular una presión
  ctlComRaspberry.enviarManometro(ctlRiego.ctlSensores.manometroSensor);
  delay(2000);

  ctlRiego.ctlSensores.tanques[1].nivel = 0; // Simular un nivel de tanque
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[1]);
  delay(2000);
  ctlRiego.ctlSensores.tanques[1].nivel = 50; // Simular un nivel de tanque
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[1]);
  delay(2000);
  // ctlComRaspberry.enviarManometro(ctlRiego.ctlSensores.manometroSensor);
  // delay(2000);
  ctlRiego.ctlSensores.tanques[1].nivel = 100; // Simular un nivel de tanque
  delay(2000);
  ctlComRaspberry.enviarTanque(ctlRiego.ctlSensores.tanques[1]);
  delay(2000);
}

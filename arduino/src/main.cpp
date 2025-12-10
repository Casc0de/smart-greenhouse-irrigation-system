#include <Arduino.h>
#include <Wire.h>

#include "Horario.h"
#include "ControladorRiego.h"
#include "ControladorComunicacionConRaspberry.h"
#include "ControladorTiempo.h"
#include "Bomba.h"

// Objetos globales
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

  if (ctlTiempo.esMomentoDeRiego())
  {
    Serial.println(">> ControladorTiempo: es momento de riego según horario");
    // ctlRiego.iniciarCiclo(); // Arranca la máquina de estados de riego

    ctlRiego.bomba.encender();         // Prueba directa de bomba
    ctlRiego.valvulasOnOff[0].abrir(); // Prueba directa de válvula On-Off
    ctlRiego.valvulasOnOff[1].abrir(); // Prueba directa de válvula On-Off
    ctlRiego.valvulasOnOff[2].abrir(); // Prueba directa de válvula On-Off
    ctlRiego.valvulasOnOff[3].abrir(); // Prueba directa de válvula On-Off
    ctlRiego.valvulasOnOff[4].abrir(); // Prueba directa de válvula On-Off
  }

  ctlRiego.valvulasEstado[0].abrir(10); // Prueba directa de válvula de estado
  ctlRiego.valvulasEstado[1].abrir(10); // Prueba directa de válvula de estado

  unsigned long ahora = millis();

  if (!activo)
  {
    // Arranca el HIGH
    ctlRiego.bomba.encender();
    activo = true;
    tInicio = ahora;

    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[0]);
    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[1]);
    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[2]);
    ctlRiego.ctlComunicacion.enviarManometro(ctlRiego.ctlSensores.manometroSensor);

    ctlRiego.ctlSensores.tanques[0].nivel = 90;
    ctlRiego.ctlSensores.tanques[1].nivel = 60;
    ctlRiego.ctlSensores.tanques[2].nivel = 30;
    ctlRiego.ctlSensores.manometroSensor.presion = 25;
  }

  //
  else if (activo && ahora - tInicio >= DURACION)
  {
    // Ya pasaron los X segundos
    ctlRiego.bomba.apagar();
    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[0]);
    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[1]);
    ctlRiego.ctlComunicacion.enviarTanque(ctlRiego.ctlSensores.tanques[2]);
    ctlRiego.ctlComunicacion.enviarManometro(ctlRiego.ctlSensores.manometroSensor);
    activo = false;
  }
}

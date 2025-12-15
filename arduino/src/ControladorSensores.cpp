#include "ControladorSensores.h"

ControladorSensores::ControladorSensores()
    : manometroSensor(A1), // Asignar el pin del manómetro
      tanques{
          Tanque(13, 14, 'M'), // Tanque para fertilizante de lunes
          Tanque(16, 14, 'W'), // Tanque para fertilizante de miércoles
          Tanque(17, 14, 'F')  // Tanque para fertilizante de viernes
      }
{
}

void ControladorSensores::begin()
{
    // Inicializar el manómetro
    manometroSensor.begin();
    // Inicializar los tanques
    for (int i = 0; i < NUM_TANQUES; ++i)
    {
        tanques[i].begin();
    }
}

void ControladorSensores::medirNivel()
{
    for (int i = 0; i < NUM_TANQUES; ++i)
    {
        tanques[i].medirNivel();
    }
}
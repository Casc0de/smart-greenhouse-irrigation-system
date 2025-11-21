#include "ControladorSensores.h"

ControladorSensores::ControladorSensores()
    : manometroSensor(20), // Asignar el pin del manómetro
      tanques{
          Tanque(16, 19, 'M'), // Tanque para fertilizante de lunes
          Tanque(21, 22, 'W'), // Tanque para fertilizante de miércoles
          Tanque(25, 26, 'F')  // Tanque para fertilizante de viernes
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
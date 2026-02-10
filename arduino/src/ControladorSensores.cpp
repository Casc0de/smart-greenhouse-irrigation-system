#include "ControladorSensores.h"

ControladorSensores::ControladorSensores()
    : manometroSensor(A1), // Asignar el pin del manómetro
      tanques{
          Tanque(13, 14, 'A'), // Tanque para fertilizante de lunes
          Tanque(16, 14, 'B'), // Tanque para fertilizante de miércoles
          Tanque(17, 14, 'C')  // Tanque para fertilizante de viernes
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
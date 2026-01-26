#include "ControladorRiego.h"

ControladorRiego::ControladorRiego(ServicioComunicacion &commsExistente)
    : bomba(14, 15),
      manometroSensor(A1),

      tanquesEstado{
          EstadoTanque('A'),  // tanque 1
          EstadoTanque('B'),  // tanque 2
          EstadoTanque('C')}, // tanque 3
      valvulasOnOff{
          ValvulaOnOff(7),   // valvula 2 : on-off
          ValvulaOnOff(8),   // valvula 3 : on-off
          ValvulaOnOff(9),   // valvula 4 : on-off
          ValvulaOnOff(12),  // valvula 6 : on-off
          ValvulaOnOff(13)}, // valvula 7 : on-off
      valvulasEstado{
          ValvulaEstado(5, 6),    // valvula 1 : estado - agua
          ValvulaEstado(10, 11)}, // valvula 5 : estado - presión general

      comms(commsExistente)
{
}

void ControladorRiego::begin()
{
    // Inicializa los pines para la bomba
    bomba.begin();
    // Inicializa los pines para el manómetro
    manometroSensor.begin();
    // Inicializa los pines para las válvulas On-Off
    for (int i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
    {
        valvulasOnOff[i].begin();
    }
    // Inicializa los pines para las válvulas de estado
    for (int i = 0; i < NUM_VALVULAS_ESTADO; ++i)
    {
        valvulasEstado[i].begin();
    }
}

// TODO: AGREGAR MÁQUINA DE ESTADOS
void ControladorRiego::regar()
{
    // Lógica de riego aquí
}
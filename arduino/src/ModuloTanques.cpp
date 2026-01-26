#include "ModuloTanques.h"

ModuloTanques::ModuloTanques()
    : tanques{
          Tanque(16, 19, 'M'), // Tanque para fertilizante de lunes
          Tanque(21, 22, 'W'), // Tanque para fertilizante de miércoles
          Tanque(25, 26, 'F')  // Tanque para fertilizante de viernes
      }
{
}

void ModuloTanques::begin()
{
    // Inicializar los tanques
    for (int i = 0; i < NUM_TANQUES; ++i)
    {
        tanques[i].begin();
    }
}

void ModuloTanques::medirNivel()
{
    for (int i = 0; i < NUM_TANQUES; ++i)
    {
        tanques[i].medirNivel();
    }
}

void ModuloTanques::medirBateria()
{
    // TODO: Implementar la medición del nivel de batería aquí
}

void ModuloTanques::enviarDatos()
{
    // TODO: Implementar el envío de datos aquí
}
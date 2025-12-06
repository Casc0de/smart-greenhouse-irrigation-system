#include "Horario.h"

Horario::Horario(char dia, uint8_t horaDeRiego, bool activo)
    : diaSemana(dia),
      horaARegar(horaDeRiego),
      fertiRiego(activo),
      yaDisparadoHoy(false)
{
}

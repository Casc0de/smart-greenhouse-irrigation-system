#include "ControladorTiempo.h"

ControladorTiempo::ControladorTiempo()
    : horarios{
          Horario('M', 6),  // Lunes a las 6 AM
          Horario('T', 6),  // Martes a las 6 AM
          Horario('W', 6),  // Miércoles a las 6 AM
          Horario('R', 6),  // Jueves a las 6 AM
          Horario('F', 6),  // Viernes a las 6 AM
          Horario('S', 6),  // Sábado a las 6 AM
          Horario('U', 6)}, // Domingo a las 6 AM
      rtc()
{
}

void ControladorTiempo::begin()
{
    rtc.begin();
}
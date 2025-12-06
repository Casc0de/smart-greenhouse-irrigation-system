/*
M = Monday
T = Tuesday
W = Wednesday
R = Thursday (to avoid confusion with Tuesday)
F = Friday
S = Saturday
U = Sunday
*/

#ifndef HORARIO_H
#define HORARIO_H

#include <stdint.h>

class Horario
{
public:
    // Letra del día de la semana:
    // 'M' = Monday, 'T' = Tuesday, 'W' = Wednesday,
    // 'R' = Thursday, 'F' = Friday, 'S' = Saturday, 'U' = Sunday.
    char diaSemana;

    // Hora a la que se debe iniciar el riego (0–23).
    // De momento sólo trabajamos con hora entera.
    uint8_t horaARegar;

    // true  = este día se riega
    // false = este día NO se riega
    bool fertiRiego;

    // Flag interno para saber si este horario ya disparó
    // el riego en el día actual (lo usamos en ControladorTiempo).
    bool yaDisparadoHoy;

    // Constructor por defecto (útil si declaras arrays sin lista completa)
    Horario()
        : diaSemana('U'),
          horaARegar(0),
          fertiRiego(false),
          yaDisparadoHoy(false)
    {
    }

    // Constructor principal: define día, hora y si ese día está activo para riego
    Horario(char dia, uint8_t horaDeRiego, bool fertiRiego);
};

#endif

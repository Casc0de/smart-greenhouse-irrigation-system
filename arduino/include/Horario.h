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
    char diaSemana; // Día de la semana
    uint8_t horaARegar;
    bool fertiRiego = false;

    Horario(char dia, uint8_t horaDeRiego); // constructor
};

#endif
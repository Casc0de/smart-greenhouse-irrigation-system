#include "ValvulaOnOff.h"

ValvulaOnOff::ValvulaOnOff(uint8_t pinOpen)
    : _pinOpen(pinOpen)
{
}

void ValvulaOnOff::begin()
{
    pinMode(_pinOpen, OUTPUT);
    // No se usa _pinShut en estas válvulas On-Off
}

// TODO: REVISAR
void ValvulaOnOff::abrir()
{
    digitalWrite(_pinOpen, LOW);
}
// TODO: REVISAR
void ValvulaOnOff::cerrar()
{
    digitalWrite(_pinOpen, HIGH);
}
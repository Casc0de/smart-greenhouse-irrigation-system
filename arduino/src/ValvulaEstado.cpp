#include "ValvulaEstado.h"

ValvulaEstado::ValvulaEstado(uint8_t pinOpen, uint8_t pinShut)
    : _pinOpen(pinOpen), _pinShut(pinShut)
{
}

void ValvulaEstado::begin()
{
    pinMode(_pinOpen, OUTPUT);
    pinMode(_pinShut, OUTPUT);
}

// TODO: REVISAR
void ValvulaEstado::abrir(uint8_t t)
{
    digitalWrite(_pinOpen, LOW);
    delay(t * 1000);
    digitalWrite(_pinOpen, HIGH);
}
// TODO: REVISAR
void ValvulaEstado::cerrar(uint8_t t)
{
    digitalWrite(_pinShut, LOW);
    delay(t * 1000);
    digitalWrite(_pinShut, HIGH);
}
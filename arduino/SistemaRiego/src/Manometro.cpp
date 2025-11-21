#include "Manometro.h"

Manometro::Manometro(uint8_t pinIn)
    : _pinIn(pinIn)
{
}

void Manometro::begin()
{
    pinMode(_pinIn, INPUT);
}

// TODO:
uint8_t Manometro::medirPresion()
{
    // Lógica en Arduino para medir la presión

    return presion;
}
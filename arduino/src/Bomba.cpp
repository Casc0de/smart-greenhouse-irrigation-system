#include "Bomba.h"

Bomba::Bomba(uint8_t pinEncender, uint8_t pinAlarma)
    : _pinEncender(pinEncender), _pinAlarma(pinAlarma)
{
}

void Bomba::begin()
{
    pinMode(_pinEncender, OUTPUT);
    pinMode(_pinAlarma, INPUT_PULLUP);
    digitalWrite(_pinEncender, HIGH); // Apagada al inicio
}

void Bomba::encender()
{
    digitalWrite(_pinEncender, LOW);
}

void Bomba::apagar()
{
    digitalWrite(_pinEncender, HIGH);
}

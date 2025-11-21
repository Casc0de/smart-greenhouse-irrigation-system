#include "Bomba.h"

Bomba::Bomba(uint8_t pinEncender,
             uint8_t pinAlarma)
    : _pinEncender(pinEncender),
      _pinAlarma(pinAlarma)
{
}

void Bomba::begin()
{
    pinMode(_pinEncender, OUTPUT);
    pinMode(_pinAlarma, OUTPUT);
}

// TODO:
void Bomba::encender()
{
    // lógica en Arduino para encender la bomba
}
// TODO:
void Bomba::apagar()
{
    // lógica en Arduino para apagar la bomba
}

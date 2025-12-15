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
    pinMode(_pinAlarma, INPUT);
    digitalWrite(_pinEncender, HIGH);
}

// TODO:
void Bomba::encender()
{   
    digitalWrite(11, LOW);
}
// TODO:
void Bomba::apagar()
{
    digitalWrite(11,HIGH);
}

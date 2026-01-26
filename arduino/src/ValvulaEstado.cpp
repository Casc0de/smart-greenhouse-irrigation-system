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

void ValvulaEstado::abrir(uint8_t t)
{
    digitalWrite(_pinOpen, LOW);

    int inicioCiclo = millis();           // Obtener el tiempo actual
    while (!activarTimer(t, inicioCiclo)) // Hasta que el timer no se dispare, siga en el ciclo preguntando
    {
        digitalWrite(_pinOpen, HIGH);
    }
}

void ValvulaEstado::cerrar(uint8_t t)
{
    digitalWrite(_pinShut, LOW);

    int inicioCiclo = millis();           // Obtener el tiempo actual
    while (!activarTimer(t, inicioCiclo)) // Hasta que el timer no se dispare, siga en el ciclo preguntando
    {
        digitalWrite(_pinShut, HIGH);
    }
}

bool ValvulaEstado::activarTimer(uint8_t tLimite, int &inicioCiclo)
{
    int ahora = millis();                      // Obtener el tiempo actual
    if (ahora - inicioCiclo >= tLimite * 1000) // Cuando se supere el tiempo límite
    {
        return true;
    }
    return false;
}
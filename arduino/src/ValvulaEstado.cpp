#include "ValvulaEstado.h"

ValvulaEstado::ValvulaEstado(uint8_t pinOpen, uint8_t pinShut)
    : _pinOpen(pinOpen), _pinShut(pinShut)
{
}

void ValvulaEstado::begin()
{
    pinMode(_pinOpen, OUTPUT);
    pinMode(_pinShut, OUTPUT);

    // Relés inactivos (HIGH = apagado para módulos low-level trigger)
    digitalWrite(_pinOpen, HIGH);
    digitalWrite(_pinShut, HIGH);

    _moviendo = false;
    _modoApertura = false;
    _tInicioMovimiento = 0;
    _tDuracionMovimiento = 0;
    posicion = 0; // quieta
}

/*
 * ABRIR – NO BLOQUEANTE
 */
void ValvulaEstado::abrir(uint8_t tSegundos)
{
    // Si tSegundos es 0, no tiene sentido moverla: la dejamos quieta.
    if (tSegundos == 0)
        return;

    _moviendo = true;
    _modoApertura = true;
    _tDuracionMovimiento = (unsigned long)tSegundos * 1000UL;
    _tInicioMovimiento = millis();

    // Activar relé de apertura, desactivar de cierre
    digitalWrite(_pinOpen, LOW);
    digitalWrite(_pinShut, HIGH);

    posicion = 1; // abriendo (debug)
}

/*
 * CERRAR – NO BLOQUEANTE
 */
void ValvulaEstado::cerrar(uint8_t tSegundos)
{
    if (tSegundos == 0)
        return;

    _moviendo = true;
    _modoApertura = false;
    _tDuracionMovimiento = (unsigned long)tSegundos * 1000UL;
    _tInicioMovimiento = millis();

    // Activar relé de cierre, desactivar de apertura
    digitalWrite(_pinShut, LOW);
    digitalWrite(_pinOpen, HIGH);

    posicion = 2; // cerrando (debug)
}

/*
 * DEBE llamarse SIEMPRE en loop() o desde regar()
 */
void ValvulaEstado::actualizar()
{
    if (!_moviendo)
        return;

    unsigned long ahora = millis();

    // Movimiento terminado
    if (ahora - _tInicioMovimiento >= _tDuracionMovimiento)
    {
        // Apagar relés
        digitalWrite(_pinOpen, HIGH);
        digitalWrite(_pinShut, HIGH);

        _moviendo = false;
        // Puedes dejar la última dirección en posicion si quieres,
        // pero para debug suele ser más claro marcarla como "quieta".
        posicion = 0;
    }
}

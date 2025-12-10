#ifndef VALVULAESTADO_H
#define VALVULAESTADO_H

#include <Arduino.h>

class ValvulaEstado
{
public:
    ValvulaEstado(uint8_t pinOpen, uint8_t pinShut);

    void begin();

    // No bloqueantes: arrancan el movimiento y retornan de inmediato.
    // tSeg = tiempo que se energiza el relé para abrir/cerrar.
    void abrir(uint8_t tSeg);
    void cerrar(uint8_t tSeg);

    // Debe llamarse en cada loop() o desde regar()
    void actualizar();

    // 0 = quieta, 1 = abriendo, 2 = cerrando
    uint8_t posicion = 0; // útil para debug / monitoreo

private:
    uint8_t _pinOpen;
    uint8_t _pinShut;

    // Estado interno no bloqueante
    bool _moviendo = false;
    bool _modoApertura = false; // true = abriendo, false = cerrando
    unsigned long _tInicioMovimiento = 0;
    unsigned long _tDuracionMovimiento = 0;
};

#endif

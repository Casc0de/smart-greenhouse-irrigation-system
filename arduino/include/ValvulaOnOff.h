#ifndef VALVULAONOFF_H
#define VALVULAONOFF_H

#include <Arduino.h>
#include <stdint.h>

class ValvulaOnOff
{
private:
    uint8_t _pinOpen;   // Pin que comanda el relé / válvula (low-level trigger)

public:
    // 0 = cerrada (relé desactivado, HIGH)
    // 1 = abierta  (relé activado, LOW)
    uint8_t posicion = 0;

    // pinOpen: pin digital conectado al módulo de relé
    ValvulaOnOff(uint8_t pinOpen);

    // Configura el pin. Deja la válvula CERRADA por defecto.
    void begin();

    // Abre la válvula (activa el relé).
    void abrir();

    // Cierra la válvula (desactiva el relé).
    void cerrar();
};

#endif

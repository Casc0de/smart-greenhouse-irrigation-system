#ifndef BOMBA_H
#define BOMBA_H

#include <Arduino.h>
#include <stdint.h>

class Bomba
{
private:
    uint8_t _pinEncender;
    uint8_t _pinAlarma;

public:
    bool funcionando = false;

    Bomba(uint8_t pinEncender,
          uint8_t pinAlarma);
    void begin();
    void encender();
    void apagar();
};

#endif
#ifndef SERVICIOCOMUNICACION_H
#define SERVICIOCOMUNICACION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Manometro.h"
#include "EstadoTanque.h"

class ServicioComunicacion
{
private:
    HardwareSerial &_serialPort;
    uint8_t _maxRetries;
    uint16_t _timeout;

public:
    ServicioComunicacion(HardwareSerial &port,
                         uint8_t retries = 3,
                         uint16_t timeoutInMs = 2000);

    bool enviarEstadoBomba(bool estadoBomba);
    bool enviarManometro(uint8_t presion);
    bool enviarTanque(uint8_t nivelTanque, char tipoFertilizanteTanque);

    bool waitForAck();
};

#endif
#ifndef CONTROLADORCOMUNICACIONCONRASPBERRY_H
#define CONTROLADORCOMUNICACIONCONRASPBERRY_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Manometro.h"
#include "Tanque.h"
#include "Bomba.h"

class ControladorComunicacionConRaspberry
{
private:
    HardwareSerial &_serialPort;
    uint8_t _maxRetries;
    uint16_t _timeout;

public:
    ControladorComunicacionConRaspberry(HardwareSerial &port,
                                        uint8_t retries = 1,
                                        uint16_t timeoutInMs = 2000);

    bool enviarManometro(const Manometro &objManometro);
    bool enviarTanque(const Tanque &objTanque);
    bool enviarEstadoBomba(const Bomba &objBomba);
    bool waitForAck();
};

#endif
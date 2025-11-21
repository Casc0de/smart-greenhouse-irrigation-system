#include "ControladorComunicacionConRaspberry.h"

ControladorComunicacionConRaspberry::ControladorComunicacionConRaspberry(HardwareSerial &port,
                                                                         uint8_t retries,
                                                                         uint16_t timeoutInMs)
    : _serialPort(port),
      _maxRetries(retries),
      _timeout(timeoutInMs)
{
}

bool ControladorComunicacionConRaspberry::enviarManometro(const Manometro &objManometro)
{
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "manometro";
        doc["presion"] = objManometro.presion;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar
        delay(1000);
    }
    // Si se agotaron los reintentos, retornar false
    return false;
}

bool ControladorComunicacionConRaspberry::enviarTanque(const Tanque &objTanque)
{
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "tanque";
        doc["nivel"] = objTanque.nivel;
        doc["tipoFertilizante"] = (String)objTanque.tipo;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar
        delay(1000);
        _serialPort.println("Reintentando enviar estado de tanque...");
    }
    // Si se agotaron los reintentos, retornar false
    return false;
}

bool ControladorComunicacionConRaspberry::enviarEstadoBomba(bool &estado)
{
    // Intentar enviar el estado de la bomba varias veces
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "bomba";
        doc["estado"] = estado;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar
        delay(1000);
        _serialPort.println("Reintentando enviar estado de bomba...");
    }
    // Si se agotaron los reintentos, retornar false
    return false;
}

// return true si recibe ACK, false si no
bool ControladorComunicacionConRaspberry::waitForAck()
{
    unsigned long start = millis();
    String buffer = "";

    while (millis() - start < _timeout)
    {
        if (_serialPort.available())
        {
            char c = _serialPort.read();
            buffer += c;

            if (buffer.indexOf("ACK") >= 0)
                return true;
        }
    }
    return false;
}
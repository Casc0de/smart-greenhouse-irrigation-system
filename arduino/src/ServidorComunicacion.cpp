#include "ServicioComunicacion.h"

ServicioComunicacion::ServicioComunicacion(HardwareSerial &port,
                                           uint8_t retries,
                                           uint16_t timeoutInMs)
    : _serialPort(port),
      _maxRetries(retries),
      _timeout(timeoutInMs)
{
}

bool ServicioComunicacion::enviarManometro(uint8_t presion)
{
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "manometro";
        doc["presion"] = presion;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

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

bool ServicioComunicacion::enviarTanque(uint8_t nivelTanque, char tipoFertilizanteTanque)
{
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "tanque";
        doc["tipoFertilizante"] = (String)tipoFertilizanteTanque;
        doc["nivel"] = nivelTanque;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar
        delay(3000);
        _serialPort.println("Reintentando enviar estado de tanque...");
    }
    // Si se agotaron los reintentos, retornar false
    return false;
}

bool ServicioComunicacion::enviarEstadoBomba(bool estadoBomba)
{
    // Intentar enviar el estado de la bomba varias veces
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "bomba";
        doc["estado"] = estadoBomba;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

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
bool ServicioComunicacion::waitForAck()
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
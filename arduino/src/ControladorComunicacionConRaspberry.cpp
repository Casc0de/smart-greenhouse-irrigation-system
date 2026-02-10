/*
========== Formato de los mensajes JSON enviados a la Raspberry Pi: ==========
{
  "tipo" : "tanque",
  "tipoFertilizante" : "A" | "B" | "C",
  "nivel" : int
}

{
    "tipo" : "manometro",
    "presion" : 25
}

{
    "tipo" : "bomba",
    "estado" : true,
    "error" : false
}
*/

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
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar

        // TODO: descomentar
        //_serialPort.println("Reintentando enviar estado de manometro...");
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final
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
        doc["tipoFertilizante"] = (String)objTanque.tipo;
        doc["nivel"] = objTanque.nivel;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar

        // TODO: descomentar
        // _serialPort.println("Reintentando enviar estado de tanque...");
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final
    }
    // Si se agotaron los reintentos, retornar false
    return false;
}

bool ControladorComunicacionConRaspberry::enviarEstadoBomba(const Bomba &objBomba)
{
    // Intentar enviar el estado de la bomba varias veces
    for (uint8_t attempt = 0; attempt < _maxRetries; ++attempt)
    {
        JsonDocument doc;
        doc["tipo"] = "bomba";
        doc["estado"] = objBomba.funcionando;
        doc["error"] = objBomba.errorAlarma;
        // serializeJson --> El primer argumento es el documento JSON, el segundo es en dónde quiero guardar/enviar el JSON
        serializeJson(doc, _serialPort);
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final

        if (waitForAck())
        {
            // ACK recibido, salir de la función
            return true;
        }
        // Si no se recibe ACK, esperar un poco antes de reintentar

        // TODO: descomentar
        //_serialPort.println("Reintentando enviar estado de bomba...");
        _serialPort.println(); // Asegurarse de enviar un salto de línea al final
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
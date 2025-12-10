# Smart Greenhouse Irrigation System

Sistema IoT para monitorear y automatizar el riego de un invernadero. El firmware en Arduino gestiona sensores, bomba y válvulas; la Raspberry Pi recibe datos por serial/MQTT, los filtra y persiste en MongoDB.

## Arquitectura

- **Nodo Arduino (PlatformIO/C++)**: controla bomba, válvulas on-off y de estado, lee manómetro y niveles de tanques de fertilizante. Envía lecturas en JSON por serial a la Raspberry (con ACK).
- **Nodo Raspberry (Python 3.11, asyncio)**: escucha MQTT, filtra datos relevantes con umbrales, persiste en MongoDB y prepara el pipeline para datos ambientales/suelo vía LoRa.

## Estructura de carpetas

- `arduino/`: proyecto PlatformIO con firmware (sensores, actuadores y comunicación serial).
- `raspberry/`: cliente Python (asyncio) con ingestión MQTT, filtrado y escritura en MongoDB.

## Firmware Arduino

- **Entrypoint**: `arduino/src/main.cpp` inicializa comunicación serial a 57600 baudios y simula lecturas de manómetro/tanques que se envían a la Raspberry.
- **Sensores/actuadores principales**:
  - Manómetro (`Manometro`): pin 20.
  - Tanques (`Tanque`): ultrasonidos en pines `(16,19)`, `(21,22)`, `(25,26)` para fertilizantes L/M/V.
  - Bomba (`Bomba`): pines 14 (encendido) y 15 (alarma).
  - Válvulas on-off (`ValvulaOnOff`): pines 7, 8, 9, 12, 13.
  - Válvulas de estado (`ValvulaEstado`): pines (5,6) y (10,11).
- **Comunicación serial**: `ControladorComunicacionConRaspberry` arma mensajes JSON y espera `ACK` antes de reintentar.
  - Manómetro: `{ "tipo": "manometro", "presion": <float> }`
  - Tanque: `{ "tipo": "tanque", "tipoFertilizante": "M|W|F", "nivel": <int> }`
  - Bomba: `{ "tipo": "bomba", "estado": true|false }`
- **Compilar/subir** (desde `arduino/` con PlatformIO):
  - `pio run` — compila
  - `pio run -t upload` — sube al microcontrolador configurado en `platformio.ini`
  - `pio device monitor -b 57600` — monitor serial

## Cliente Raspberry (Python)

- **Requisitos**: Python 3.11+, Poetry. Dependencias clave: `aiomqtt`, `motor` (Mongo), `paho-mqtt`, `python-dotenv`, `pyserial`, `aioserial`.
- **Instalación**:
  ```powershell
  cd raspberry
  poetry install
  ```
- **Variables de entorno** (ver `raspberry/src/raspberry/env.py`):
  - `MONGO_HOST` (def: `localhost`), `MONGO_PORT` (def: `27017`), `MONGO_USER`, `MONGO_PASSWORD`, `MONGO_DB` (def: `invernadero`).
  - `MQTT_HOST` (def: `127.0.0.1`), `MQTT_PORT` (def: `1883`).
  - `SERIAL_PORT` (def: `COM5` en Windows), `BAUDRATE` (def: `57600`).
- **Tópicos MQTT** (en `config.StaticConfig`):
  - `invernadero/tanque`, `invernadero/manometro`, `invernadero/bomba`.
  - Ambientales LoRa: `application/b15867e8-a061-49d9-893b-c73112deb1a4/device/+/event/up`.
  - Suelo LoRa: `application/3d192c70-c2df-4b6d-9464-ae69106686fa/device/+/event/up`.
- **Flujo asíncrono** (`raspberry.main`):
  - `MQTTListener` suscribe y enruta mensajes a colas específicas.
  - `DataFilter` convierte payloads a dicts, aplica umbrales (`THRESHOLDS`) y añade timestamp UTC antes de mandar a la cola de BD.
  - `DatabaseWriter` inserta en colecciones Mongo: `tank_levels`, `irrigation_pressure`, `environment_readings`, `soil_readings`.
- **Ejecutar**:
  ```powershell
  cd raspberry
  poetry run raspberry   # o: poetry run python -m raspberry.main
  ```

## Flujo de datos (resumen)

Arduino → JSON por serial → Raspberry (aiomqtt + colas asyncio) → filtro por umbral → inserción en MongoDB → análisis/visualización (fuera de alcance).

## Notas y pendientes

- Varias funciones Arduino marcadas como TODO (lógica real de medición y control).
- `SerialListener` está listo para habilitar comunicación serial (comentado en `main.py`).
- Ajusta umbrales en `config.py` según sensibilidad deseada.

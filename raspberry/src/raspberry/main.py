import asyncio
import os
import sys
import aiomqtt

from serial_listener import SerialListener
from mqtt_listener import MQTTListener
from data_filter import DataFilter

async def main():
    # queues para la comunicación entre tareas ------------------------
    filter_queue = asyncio.Queue()
    db_queue = asyncio.Queue()
    
    
    # Inicializar componentes ------------------------------------------
    serial_listener = SerialListener(puerto="COM5", baudrate=57600)
    print(serial_listener)
    mqtt_listener = MQTTListener(filter_queue)
    print(mqtt_listener)
    #"""
    data_filter = DataFilter(filter_queue, db_queue, thresholds={
        "tanque/M": 5,      # Umbral para nivel de tanque
        "tanque/W": 5,      # Umbral para nivel de tanque
        "tanque/F": 5,      # Umbral para nivel de tanque
        "manometro": 2,   # Umbral para presión del manómetro
        "bomba": 1        # Umbral para estado de la bomba
    })
    #"""

    # Tareas concurrentes -------------------------------------
    # Conectarse al broker mqtt
    async with aiomqtt.Client(hostname="127.0.0.1", port=1883) as client:
        print("Conectado al broker MQTT")
        # Ejecutar las tareas concurrentes
        await asyncio.gather(
            serial_listener.recibir_datos_arduino(client),
            mqtt_listener.recibir_dato(client),
            data_filter.filtrar_datos()
        )

if __name__ == "__main__":
    if sys.platform.lower() == "win32" or os.name.lower() == "nt":
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
    asyncio.run(main())
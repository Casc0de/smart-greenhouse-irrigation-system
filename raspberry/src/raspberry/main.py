import asyncio
import os
import sys
import aiomqtt


from raspberry.serial_listener import SerialListener
from raspberry.mqtt_listener import MQTTListener
from raspberry.data_filter import DataFilter
from raspberry.database_writer import DatabaseWriter
from raspberry.config import StaticConfig


async def _main():
    # queues para la comunicación entre tareas ------------------------
    tank_data_queue = asyncio.Queue()
    pressure_data_queue = asyncio.Queue()
    environment_data_queue = asyncio.Queue()
    soil_data_queue = asyncio.Queue()

    db_queue = asyncio.Queue()
    
    # Inicializar componentes asíncronos -------------------------------
    serial_listener = SerialListener(puerto=StaticConfig.SERIAL_PORT, 
                                     baudrate=StaticConfig.BAUDRATE)
    mqtt_listener = MQTTListener(tank_data_queue, 
                                 pressure_data_queue, 
                                 environment_data_queue, 
                                 soil_data_queue)
    data_filter = DataFilter(tank_data_queue, 
                             pressure_data_queue, 
                             environment_data_queue,
                             soil_data_queue,
                             db_queue, 
                             THRESHOLDS=StaticConfig.THRESHOLDS)
    database_writer = DatabaseWriter(db_queue, 
                                     MONGO_URI=StaticConfig.MONGO_URI, 
                                     DB_NAME=StaticConfig.DB_NAME)

    # Tareas concurrentes -------------------------------------
    # Conectarse al broker mqtt
    print("Conectando al broker MQTT...")
    async with aiomqtt.Client(hostname=StaticConfig.MQTT_HOST, 
                              port=StaticConfig.MQTT_PORT) as client:
        print("Conectado al broker MQTT")
        # Ejecutar las tareas concurrentes
        await asyncio.gather(
            #serial_listener.recibir_datos_arduino(client),     # TODO: descomentar
            mqtt_listener.recibir_dato(client),
            data_filter.tank_processor(),
            data_filter.pressure_processor(),
            data_filter.environment_processor(),
            data_filter.soil_processor(),
            database_writer.start()
        )

def main():
    """Entrypoint sincrónico para los scripts/console_scripts.

    Poetry/console scripts no awaitean coroutines, así que exponemos una
    función sincronica `main()` que ejecuta la coroutine `_main()` con
    `asyncio.run()`.
    """
    if sys.platform.lower() == "win32" or os.name.lower() == "nt":
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
    asyncio.run(_main())


if __name__ == "__main__":
    main()
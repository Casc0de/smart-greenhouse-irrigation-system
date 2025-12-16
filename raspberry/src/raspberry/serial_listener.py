"""
Se reciben datos tipo JSON desde Arduino a través del puerto serial.
Estos datos son clasificados y publicados en tópicos MQTT correspondientes.

Source_input: Arduino
Source_output: MQTT Broker

Formato de datos recibidos desde Arduino (JSON):
{
    "tipo": "tanque",
    "tipoFertilizante": "M" | "B" | "C",  # Solo para tipo "tanque"
    "nivel": float,                        # Solo para tipo "tanque"
    "presion": float
}

{
    "tipo":"manometro",
    "presion":25
}

"""


import json
import asyncio
from aioserial import AioSerial
import logging


class SerialListener:
    def __init__(self, puerto, baudrate):
        self.puerto = puerto
        self.baudrate = baudrate
        self.ser = None
        self.mqtt_client = None
        

    async def recibir_datos_arduino(self, mqtt_client):

        self.mqtt_client = mqtt_client

        while True:         # Se encarga de que la conexión serial esté siempre activa
            try:
                await self.connect_serial() # Asegura que la conexión serial esté establecida
                logging.info(f"[Serial_Listener] \n Conectado al puerto serial {self.puerto} \n")
                
                #logging.info(f"Conectado al puerto serial {self.puerto}")

                while True:
                    linea_bytes = await self.ser.readline_async()
                    logging.info(f"[Serial_Listener] \n Recibido de Arduino: {linea_bytes} \n")

                    self.ser.write(b'ACK\n')  # Enviar recibido

                    linea = linea_bytes.decode().strip()

                    data = json.loads(linea) # Convierte el string JSON a un diccionario de Python para su poder manipularse

                    # Clasificación de datos
                    datos_clasificados = self.clasificar_mensaje(data)
                    datos_clasificados_json = json.dumps(datos_clasificados)
                    
                    await self.mqtt_client.publish(datos_clasificados["topico"], datos_clasificados_json)  # Publica el dato en el tópico correspondiente
                    
            except Exception as e:
                logging.error(f"[Serial_Listener] \n Error en la conexión serial: Mensaje no reconocido para procesar.\n Cerrando y reintentando en 10 segundos... \n")
                
                try:
                    self.ser.close()
                except:
                    pass
                self.ser = None

                await asyncio.sleep(10)  # Esperar antes de reintentar

    async def connect_serial(self):
        while True:
            try:
                self.ser = AioSerial(port=self.puerto, baudrate=self.baudrate)
                logging.info(f"[Serial_Listener] \n Serial conectado en {self.puerto} \n")
                return
            except Exception as e:
                logging.info("[Serial_Listener] \n ")
                logging.error(f"[Serial_Listener] \n Error al conectar con el puerto serial: {e}. Reintentando en 10 segundos... \n")
                await asyncio.sleep(10)  # Esperar antes de reintentar


    def clasificar_mensaje(self, datos):
        if datos["tipo"] == "tanque":
            datos["topico"] = f"invernadero/tanque/{datos['tipoFertilizante']}"
        elif datos["tipo"] == "manometro":
            datos["topico"] = "invernadero/manometro/presion"            
        elif datos["tipo"] == "bomba":
            datos["topico"] = "invernadero/bomba/estado"
        
        

        return datos
    
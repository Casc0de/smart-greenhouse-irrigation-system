import os
from dotenv import load_dotenv

load_dotenv()

class EnvConfig:
    # Si no se encuentran las variables de entorno, se usan valores por defecto (localhost, 27017, etc.)
    
    # Configuración de MongoDB
    MONGO_HOST = os.getenv("MONGO_HOST", "localhost")
    MONGO_PORT = int(os.getenv("MONGO_PORT", 27017))
    MONGO_USER = os.getenv("MONGO_USER")
    MONGO_PASSWORD = os.getenv("MONGO_PASSWORD")
    MONGO_DB = os.getenv("MONGO_DB", "invernadero")

    # Configuración de MQTT
    MQTT_HOST = os.getenv("MQTT_HOST", "127.0.0.1")
    MQTT_PORT = int(os.getenv("MQTT_PORT", 1883))

    # Configuración del puerto serial
    SERIAL_PORT = os.getenv("SERIAL_PORT", "COM5")
    BAUDRATE = int(os.getenv("BAUDRATE", 57600))
from raspberry.env import EnvConfig

class StaticConfig:
    # MQTT Topics
    TANK_TOPIC = "invernadero/tanque"
    MANOMETER_TOPIC = "invernadero/manometro"
    PUMP_TOPIC = "invernadero/bomba"
    ENVIRONMENT_TOPIC = "application/b15867e8-a061-49d9-893b-c73112deb1a4/device/+/event/up"
    SOIL_TOPIC = "application/3d192c70-c2df-4b6d-9464-ae69106686fa/device/+/event/up"

    # Data filtering thresholds
    THRESHOLDS = {
        "tanque": 5,      # Umbral para nivel de tanque
        "manometro": 2,   # Umbral para presión del manómetro
        "bomba": 1,        # Umbral para estado de la bomba
        "air_temp": 0,  # Umbral para temperatura ambiente
        "air_hum": 0,   # Umbral para humedad ambiente
        "co2": 0,      # Umbral para CO2
        "soil_moisture": 0,  # Umbral para humedad del suelo
        "soil_temp": 0   # Umbral para temperatura del suelo
        }
    
    # Database configuration
    MONGO_URI = f"mongodb://{EnvConfig.MONGO_USER}:{EnvConfig.MONGO_PASSWORD}@{EnvConfig.MONGO_HOST}:{EnvConfig.MONGO_PORT}"
    DB_NAME = EnvConfig.MONGO_DB

    # MQTT configuration
    MQTT_HOST = EnvConfig.MQTT_HOST
    MQTT_PORT = EnvConfig.MQTT_PORT
    
    # Serial configuration
    SERIAL_PORT = EnvConfig.SERIAL_PORT
    BAUDRATE = EnvConfig.BAUDRATE

    
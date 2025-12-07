from raspberry.env import EnvConfig

class StaticConfig:
    # MQTT Topics
    TANK_TOPIC = "invernadero/tanque"
    MANOMETER_TOPIC = "invernadero/manometro"
    PUMP_TOPIC = "invernadero/bomba"
    ENVIRONMENT_TOPIC = "application/4370abaa-8c04-4dd9-9eb8-ba143cfcb431/device/"
    SOIL_TOPIC = "application/e46d11f6-704a-4330-ab14-539b3c9d1a72/device/"

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

    
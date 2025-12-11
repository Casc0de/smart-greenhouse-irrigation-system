from datetime import datetime, timezone
import json

class DataFilter:
    def __init__(self, 
                 tank_data_queue, 
                 pressure_data_queue, 
                 environment_data_queue,
                 soil_data_queue,
                 db_queue, 
                 THRESHOLDS):
        self.tank_data_queue = tank_data_queue
        self.pressure_data_queue = pressure_data_queue
        self.environment_data_queue = environment_data_queue
        self.soil_data_queue = soil_data_queue
        self.db_queue = db_queue
        self.THRESHOLDS = THRESHOLDS  # Diccionario con los umbrales para cada tipo de dato
        self.last_values = {}  # Para almacenar los últimos valores recibidos

    async def tank_processor(self):
        while True:
            tank = await self.tank_data_queue.get()     # Esperar a recibir un mensaje de tanque
            tank_dic = self.mqtt_to_dict(tank)          # Convertir el mensaje MQTT a diccionario
            print(f"Procesando mensaje de tanque: {tank_dic}")

            tank_level = tank_dic["nivel"]             # Obtener el nivel del tanque del diccionario
            tank_tipo = tank_dic["tipo"]
            
            # Filtrar mensaje de tanque ------------------------
            if self.is_relevant(tank_tipo, tank_level):
                
                tank_dic["tiempo"] = self.get_timestamp_utc()
                print(f"Tiempo es {tank_dic['tiempo']}")

                await self.db_queue.put({"tiempo": tank_dic["tiempo"],
                                         "tipoFertilizante": tank_dic["tipoFertilizante"],
                                         "nivel": tank_dic["nivel"],
                                         "collection": "tank_levels"})      # Se envía el diccionario al queue de la base de datos
            else:
                print(f"Mensaje de tanque descartado por el filtro: {tank_dic}")

    async def pressure_processor(self):
        while True:
            pressure = await self.pressure_data_queue.get()
            pressure_dic = self.mqtt_to_dict(pressure)
            print(f"Procesando mensaje de manómetro: {pressure_dic}")

            pressure_value = pressure_dic["presion"]
            pressure_tipo = pressure_dic["tipo"]

            if self.is_relevant(pressure_tipo, pressure_value):
                pressure_dic["tiempo"] = self.get_timestamp_utc()
                print(f"Tiempo es {pressure_dic['tiempo']}")

                await self.db_queue.put({"tiempo": pressure_dic["tiempo"],
                                         "presion": pressure_dic["presion"],
                                         "collection": "irrigation_pressure"})
            else:
                print(f"Mensaje de manómetro descartado por el filtro: {pressure_dic}")
    
    async def environment_processor(self):
        while True:
            environment = await self.environment_data_queue.get()
            environment_dic = self.mqtt_to_dict(environment)

            # Extraer valores de las mediciones del mensaje LoRa
            air_temp = None
            air_humidity = None
            air_co2 = None
            
            try:
                print("Procesando mensaje ambiental...")
                for message in environment_dic["object"]["messages"]:
                    if "measurementId" in message:
                        if message["measurementId"] == 4097:  # Temperature
                            air_temp = message["measurementValue"]
                        elif message["measurementId"] == 4098:  # Humidity
                            air_humidity = message["measurementValue"]
                        elif message["measurementId"] == 4100:  # CO2
                            air_co2 = message["measurementValue"]

                print(f"Air Temperature: {air_temp}, Air Humidity: {air_humidity}, Air CO2: {air_co2}")

                timestamp_utc = self.get_timestamp_utc()
                await self.db_queue.put({"tiempo": timestamp_utc,
                                        "temperatura_aire": air_temp,
                                        "humedad_aire": air_humidity,
                                        "co2_aire": air_co2,
                                        "collection": "environment_readings"})
            except Exception as e:
                print(f"[DATA_Filter] \n [WARN] Error al procesar mensaje ambiental: {e} \n No se encontró {e} en el mensaje ambiental: \n{environment_dic} ")
            

            """ # TODO: AGREGAR FILTROS DE RELEVANCIA A LOS DATOS AMBIENTALES
            air_temp_relevant = self.is_relevant("air_temp", air_temp)
            air_humidity_relevant = self.is_relevant("air_humidity", air_humidity)
            air_co2_relevant = self.is_relevant("air_co2", air_co2)

            if air_temp_relevant or air_humidity_relevant or air_co2_relevant:
                
                print(f"Tiempo es {timestamp_utc}")
            """
                
    async def soil_processor(self):
        while True:
            soil = await self.soil_data_queue.get()
            soil_dic = self.mqtt_to_dict(soil)

            # Extraer valores de las mediciones del mensaje LoRa
            soil_moisture = None
            soil_temperature = None
            try:
                print("Procesando mensaje del suelo...")
                for message in soil_dic["object"]["messages"]:
                    if "measurementId" in message:
                        if message["measurementId"] == 4102:  # Soil Temperature
                            soil_temperature = message["measurementValue"]
                        elif message["measurementId"] == 4103:  # Soil Moisture
                            soil_moisture = message["measurementValue"]
                            
                print(f"Soil Moisture: {soil_moisture}, Soil Temperature: {soil_temperature}")

                timestamp_utc = self.get_timestamp_utc()
                await self.db_queue.put({"tiempo": timestamp_utc,
                                        "humedad_suelo": soil_moisture,
                                        "temperatura_suelo": soil_temperature,
                                        "collection": "soil_readings"})
            except Exception as e:
                print(f"[DATA_Filter] \n [WARN] Error al procesar mensaje del suelo: {e} \n No se encontró {e} en el mensaje del suelo: \n{soil_dic} ")

    def is_relevant(self, type, value):
        
        print(f"[DATA_filter]")
        print(f" tipo: {type} \n valor: {value}")
        print(f" umbral: {self.THRESHOLDS[type]} \n ultimo valor: {self.last_values.get(type, 'N/A')}")
        if type not in self.last_values:
            self.last_values[type] = value
            return True  # Primer dato, siempre pasa el filtro
        

        if abs(value - self.last_values[type]) >= self.THRESHOLDS[type]:
            self.last_values[type] = value
            print("Dato relevante. Procede a enviarse a la BD")
            return True  # El dato pasa el filtro
        print("Dato no relevante. No se envía a la BD")
        return False  # El dato no pasa el filtro

    def get_timestamp_utc(self):
        # Normalizar/convertir el campo 'tiempo' a un datetime con zona UTC
        raw_time = datetime.now(timezone.utc)
        if isinstance(raw_time, str):
            try:
                dt = datetime.fromisoformat(raw_time)
                if dt.tzinfo is None:
                    dt = dt.replace(tzinfo=timezone.utc)
                else:
                    dt = dt.astimezone(timezone.utc)
            except Exception:
                dt = datetime.now(timezone.utc)
        elif isinstance(raw_time, datetime):
            dt = raw_time.astimezone(timezone.utc) if raw_time.tzinfo else raw_time.replace(tzinfo=timezone.utc)
        else:
            dt = datetime.now(timezone.utc)

        return dt
    
    def mqtt_to_dict(self, mqtt_message):           # Se obtiene el mensaje de mqtt: objeto MQTT
        decoded = mqtt_message.payload.decode()     # Decodificar el payload: objeto --> string
        return json.loads(decoded)                  # Convertir string (con estructura de JSON) a diccionario   

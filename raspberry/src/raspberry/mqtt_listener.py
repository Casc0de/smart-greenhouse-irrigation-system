import logging

class MQTTListener:
    def __init__(self, tank_data_queue, pressure_data_queue, environment_data_queue, soil_data_queue):
        # MQTT client placeholder
        self.mqtt_client = None
        # Queues para diferentes tipos de datos
        self.tank_data_queue = tank_data_queue
        self.pressure_data_queue = pressure_data_queue
        self.environment_data_queue = environment_data_queue      # TODO: para los datos ambientales del sensor LoRa
        self.soil_data_queue = soil_data_queue             # TODO: para los datos del suelo del sensor LoRa
        

    async def recibir_dato(self, cliente_mqtt, tank_topic, pressure_topic, environment_topic, soil_topic):
        self.mqtt_client = cliente_mqtt
        
        # Suscribirse al tópico Invernadero_Edier
        await self.mqtt_client.subscribe("invernadero/#")
        print("Escuchando en tópico: invernadero/#")
        await self.mqtt_client.subscribe("application/+/device/+/event/up")  # Tópico para datos ambientales LoRa
        print("Escuchando en tópico: application/+/device/+/event/up")
        logging.info("")

        async for mensaje in self.mqtt_client.messages:
            topic = mensaje.topic.value
            print(f"\n[MQTT_Listener] \n Mensaje recibido en tópico: {topic}\n")
            logging.info("")
            
            if topic.startswith(tank_topic):
                await self.tank_data_queue.put(mensaje)
            elif topic.startswith(pressure_topic):
                await self.pressure_data_queue.put(mensaje)
            elif topic.startswith(environment_topic):
                await self.environment_data_queue.put(mensaje)
            elif topic.startswith(soil_topic):
                await self.soil_data_queue.put(mensaje)

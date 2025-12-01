class MQTTListener:
    def __init__(self, tank_data_queue, pressure_data_queue, environment_data_queue, soil_data_queue):
        # MQTT client placeholder
        self.mqtt_client = None
        # Queues para diferentes tipos de datos
        self.tank_data_queue = tank_data_queue
        self.pressure_data_queue = pressure_data_queue
        self.environment_data_queue = environment_data_queue      # TODO: para los datos ambientales del sensor LoRa
        self.soil_data_queue = soil_data_queue             # TODO: para los datos del suelo del sensor LoRa
        

    async def recibir_dato(self, cliente_mqtt):
        self.mqtt_client = cliente_mqtt
        
        # Suscribirse al tópico Invernadero_Edier
        await self.mqtt_client.subscribe("invernadero/#")
        print("Escuchando en tópico: invernadero/#")
        await self.mqtt_client.subscribe("application/+/device/+/event/up")  # Tópico para datos ambientales LoRa
        print("Escuchando en tópico: application/+/device/+/event/up")

        async for mensaje in self.mqtt_client.messages:
            topic = mensaje.topic.value
            print(f"\n[MQTT_Listener] \n Mensaje recibido en tópico: {topic}\n")
            
            if topic.startswith("invernadero/tanque"):
                await self.tank_data_queue.put(mensaje)
            elif topic.startswith("invernadero/manometro"):
                await self.pressure_data_queue.put(mensaje)
            elif topic.startswith("application/b15867e8-a061-49d9-893b-c73112deb1a4/device/"):
                await self.environment_data_queue.put(mensaje)
            elif topic.startswith("application/3d192c70-c2df-4b6d-9464-ae69106686fa/device/"):
                await self.soil_data_queue.put(mensaje)

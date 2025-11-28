class MQTTListener:
    def __init__(self, filter_queue, mqtt_client = None):
        self.filter_queue = filter_queue
        self.mqtt_client = mqtt_client

    async def recibir_dato(self, cliente_mqtt):
        self.mqtt_client = cliente_mqtt
        print("Escuchando en tópico: invernadero/#")
        # Suscribirse al tópico Invernadero_Edier
        await self.mqtt_client.subscribe("invernadero/#")

        async for mensaje in self.mqtt_client.messages:
            #print(f"[MQTT_Listener] \n Topico: {mensaje.topic} \n Valor: {mensaje.payload}")
            #print(f"Mensaje recibido en tópico {mensaje.topic}: {mensaje.payload.decode()} \n")
            
            payload = {
                "topico": mensaje.topic.value,
                "valor": int(mensaje.payload.decode())
            }
            #print(f"Payload de mensaje a enviar al filtro: \n {payload["topico"]}")
            await self.filter_queue.put(payload)

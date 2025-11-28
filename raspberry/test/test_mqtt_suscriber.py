import paho.mqtt.client as mqttclient
import time

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado al broker MQTT")
        global connected
        connected = True
    else:
        print("Fallo en la conexion, codigo de resultado:", rc)

def on_message(client, userdata, message):
    print("Mensaje recibido en topic:", message.topic)
    print("Contenido del mensaje:", str(message.payload.decode("utf-8")))
    global MessageReceived
    MessageReceived = True

connected = False
MessageReceived = False
broker_address = "localhost"
port = 1883
user = ""
password = ""

client = mqttclient.Client()
client.on_message = on_message
client.username_pw_set(user, password=password)
client.on_connect = on_connect
client.connect(broker_address, port=port)

# Start the network loop once and keep it running while we receive multiple messages.
client.loop_start()

# Wait until connected before subscribing
while not connected:
    time.sleep(0.1)

client.subscribe("test/topic1")

for i in range(3):
    print(i)

    # reset the message flag and wait for the next message
    MessageReceived = False
    while not MessageReceived:
        time.sleep(0.1)

# Stop loop and disconnect cleanly
client.loop_stop()
client.disconnect()
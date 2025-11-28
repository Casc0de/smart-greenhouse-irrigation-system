import paho.mqtt.publish as publish
import json
import time

# Define the MQTT broker address and port
broker_address = "localhost"
broker_port = 1883

# Define the topic
topic = "test/topic1"

# Define the messages to be sent alternately
messages = [{"state": True}, {"state": False}]

# Publish messages periodically every 3 seconds
while True:
    for message in messages:
        # Convert the message to JSON format
        payload = json.dumps(message)
        payload2 = 3

        # Publish the message
        publish.single(topic, payload2, hostname=broker_address,
                       port=broker_port)

        print(f"Published message on topic '{topic}': {payload2}")

        # Wait for 3 seconds before sending the next message
        time.sleep(1)
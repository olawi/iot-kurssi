import time, datetime
import json

import paho.mqtt.client as mqtt

from home_DHT import init_mqtt, mqtt_listen, mqtt_client, mqtt_request_str
from home_DHT import connect_mongo, write_to_db

USE_MONGO = True
try:
    import pymongo
except:
    USE_MONGO = False

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)


def on_message(client, userdata, msg):
    print("Got MQTT message ")
    data = msg.payload.decode('UTF-8')
    write_to_db(data)

if __name__ == "__main__":
    print("Init MQTT listener and DB writer")

    init_mqtt()
    mqtt_client.subscribe("olawi/homeDHT/data")
    mqtt_client.on_message = on_message
    connect_mongo()

    mqtt_listen()


import os
import sys
import time, datetime
import json
import re
import psutil

import paho.mqtt.client as mqtt
import gpiozero

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

''' MQTT '''
""" import these variables as needed """
broker_address = "broker.hivemq.com"
mqtt_request_str = "olawi/rasp_pins/request"
mqtt_publish_str = "olawi/rasp_pins/data"

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    """ read data and send away """
    print(f"Received MQTT request:{msg.payload.decode('UTF-8')}")
    data = {"REQ" : msg.payload.decode('UTF-8')}
    data.update(do_measure())
    print(data)
    client.publish(mqtt_publish_str, json.dumps(data))

mqtt_client = mqtt.Client()

def init_mqtt():
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
    mqtt_client.connect(broker_address)

def mqtt_listen():
    try:
        mqtt_client.loop_forever()
    except:
        print("EXITED loop_forever")

def do_measure():
    """ Return some hw measurements for good measure """
    data = {}
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")
    data.update({"timestamp" : timestamp})

    led = gpiozero.LED("BOARD7")
    status = led.is_active
    data.update({f"LED {led}" : status})

    return data

''' QUICK and DIRTY MAIN '''
if __name__ == "__main__":
    """ Test the functions from this file """

    os_info = os.uname()
    print(f'running on {os_info}')
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")    
    print(f'the time is {timestamp}')

    print(json.dumps(do_measure()))

    init_mqtt()
    mqtt_listen()

    print("Exiting...")


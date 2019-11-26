import os
import time
import json
import paho.mqtt.client as mqtt

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

''' MQTT '''
from rasp_hw import broker_address, mqtt_request_str, mqtt_publish_str

def on_connect(client, userdata, flags, rc):
    """ subscribe to the data response channel """
    client.subscribe(mqtt_publish_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    """ read and process data """
    print("Received MQTT publish response")
    print(json.loads(msg.payload))

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

if __name__ == "__main__":
    
    init_mqtt()
    pid = os.fork()

    if pid == 0:
        """ set up the listener for callback in on_message """
        debug("Child process listening...")
        mqtt_listen()
    else:
        debug("Parent sending messages...")
        msg_no = 0
        while True:
            """ request data every now and then """
            time.sleep(3)
            print(f"sending {mqtt_request_str} #{msg_no}")
            mqtt_client.publish(mqtt_request_str, f"DATA REQ {msg_no}")
            msg_no = msg_no + 1




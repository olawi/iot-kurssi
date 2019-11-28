import os
import sys
import time, datetime
import random
import json
import re
import psutil

import paho.mqtt.client as mqtt
from gpiozero import PWMLED


""" Define the GPIO parameters """
RED_LED_PIN = 1
GRN_LED_PIN = 3
BLU_LED_PIN = 4
PWM_FREQ = 120

red_led = PWMLED(RED_LED_PIN, frequency = PWM_FREQ, initial_value = 0)
grn_led = PWMLED(GRN_LED_PIN, frequency = PWM_FREQ, initial_value = 0)
blu_led = PWMLED(BLU_LED_PIN, frequency = PWM_FREQ, initial_value = 0)

leds = [red_led, grn_led, blu_led]

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

def f28bit(x):
    return (x*255).__round__()

''' MQTT '''
""" import these variables as needed """
broker_address = "broker.hivemq.com"
mqtt_request_str = "olawi/led_controller/request"
mqtt_publish_str = "olawi/led_controller/data"

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    """ read request and adjust led(s) """
    debug(f"Received MQTT request:{msg.payload.decode('UTF-8')}")

    """ parse input """

    do_rgb()

    """ return LED momentary status """
    reply = {}
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")
    reply.update({"timestamp" : timestamp})    

    reply.update({"R" : f28bit(red_led.value)})
    reply.update({"G" : f28bit(grn_led.value)})
    reply.update({"B" : f28bit(blu_led.value)})
    
    debug(reply)
    client.publish(mqtt_publish_str, json.dumps(reply))    

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

def init_leds():
    """ Initialize HW """

def do_rgb():
    """ Adjust GPIO pins accorging to input """
    data = {}
    data.update({"R" : red_led.value})
    data.update({"G" : grn_led.value})
    data.update({"B" : blu_led.value})

    debug(json.dumps(data))
    return data

''' MAIN '''
if __name__ == "__main__":
    """ Enter simple control loop  """

    os_info = os.uname()
    print(f'running on {os_info}')
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")    
    print(f'the time is {timestamp}')

    """ start random pulsing """
    for led in leds: 
        led.pulse(fade_in_time=random.random()+.20, fade_out_time=random.random()+.20)

    init_mqtt()
    mqtt_listen()

    for led in leds: led.off()

    print("Exiting RGB control")


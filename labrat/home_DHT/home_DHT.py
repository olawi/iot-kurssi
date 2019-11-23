import os
import sys
import time, datetime
import json
import re

import paho.mqtt.client as mqtt

USE_MONGO = True
try:
    import pymongo
except:
    USE_MONGO = False

USE_DHT = True
try:
    import Adafruit_DHT 
except:
    USE_DHT = False

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

''' MQTT '''
""" import these variables as needed """
broker_address = "broker.hivemq.com"
mqtt_request_str = "olawi/homeDHT/request"
mqtt_publish_str = "olawi/homeDHT/data"

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    """ devices will override this function """
    print("Received MQTT request")
    print(msg.payload)
    client.publish(mqtt_publish_str,'--- TESTING ---')

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

''' MONGO '''
if USE_MONGO:
    db_client = pymongo.MongoClient()

def connect_mongo():
    """ Read credentials from local file .mongopass and connect to database and collection """
    #Read login info from local file
    fd = open(".mongopass")
    mongoargs = []
    for i in fd.readlines():
        mongoargs.append(i.strip())
        fd.close()

    #Establish connection to MongoDB
    client = pymongo.MongoClient(mongoargs[0])

    #Db and Collection
    mydb = client[mongoargs[1]]
    mycol = mydb[mongoargs[2]]

    #Returns handle to the collection
    return mycol

def write_to_db(data):
    """ write json to DB """
    db = connect_mongo()
    debug('Writing '+data+' to DB...')
    db.insert_one(json.loads(data))
    debug('success!')
    db_client.close()

''' MAIN for testing '''
if __name__ == "__main__":
    """ Test the functions from this file """

    print(f'DHT device driver present: {USE_DHT}')
    print(f'mongoDB usable: {USE_MONGO}')
    print(f'running on {os.uname()}')

    init_mqtt()

    ''' test DB '''
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")    
    data = f'{{"tag":"test","data":7.9,"time":"{timestamp}"}}'
    if USE_MONGO:
        write_to_db(data)

    mqtt_listen()

    print("Exiting...")


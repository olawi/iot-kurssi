import sys
import time
import pymongo
#import Adafruit_DHT #uncomment this line if you have DHT sensor connected to RPi
import paho.mqtt.client as mqtt
import json
import datetime
import re

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

USE_MONGO = 1

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
    db_client = pymongo.MongoClient(mongoargs[0])

    #Db and Collection
    mydb = db_client[mongoargs[1]]
    mycol = mydb[mongoargs[2]]

    #Returns handle to the collection
    return mycol

def close_mongo():
    db_client.close() 

def write_to_db(data):
    """ write json to DB """
    db = connect_mongo()
    debug('Writing '+data+' to DB...')
    db.insert_one(json.loads(data))
    debug('success!')
    close_mongo()

""" Global variables for MQTT """
broker_address = "broker.hivemq.com"
mqtt_request_str = "oksanen/willab"
mqtt_publish_str = "oksanen/willab/ID1"


mqtt_client = mqtt.Client()
id = "ID1"

def on_connect(client, userdata, flags, rc):
    mqtt_client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    print("Received MQTT request")
    print(msg.payload)

    temperature = 0.666
    humidity = 0.777
 #    humidity, temperature = Adafruit_DHT.read_retry(11, 4)

    # HOX: this fails if there is no \d+ in msp.payload
    req = re.findall(r'\d+', str(msg.payload))[0]
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

    reply = '{{"Req":{},"Time":"{}","ID":"{}","T":{:0.1f},"H":{:0.1f}}}'.format(req, timestamp, id, temperature, humidity)

    print(reply)
    mqtt_client.publish(mqtt_publish_str, reply)

    if USE_MONGO:
        write_to_db(reply)

    print('end of on_message')

while(1):
    dummy_data = '{"tag":"test","data":3.11}'
    print("Write")
    write_to_db(dummy_data)
    print("OK, waiting...")
    time.sleep(5)

mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.connect(broker_address)
def do_measure():

    humidity, temperature = Adafruit_DHT.read_retry(11, 4)
    req = 71177
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

    reply = '{{"Req":{},"Time":"{}","ID":"{}","T":{:0.1f},"H":{:0.1f}}}'.format(req, timestamp, id, temperature, humidity)

    print(reply)

    if USE_MONGO:
        write_to_db(reply)

    print('end of do_measure')

try:
    mqtt_client.loop_forever()
except:
    print("error")

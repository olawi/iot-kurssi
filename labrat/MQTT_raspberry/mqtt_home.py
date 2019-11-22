import sys
import time
import pymongo
import Adafruit_DHT #uncomment this line if you have DHT sensor connected to RPi
import paho.mqtt.client as mqtt
import datetime
import re

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

USE_MONGO = 1

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
    db.close()

""" Global variables for MQTT """
broker_address = "broker.hivemq.com"
mqtt_request_str = "oksanen/willab"
mqtt_publish_str = "oksanen/willab/ID1"


client = mqtt.Client()
id = "ID1"

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    print("Received MQTT request")
    print(msg.payload)

    humidity, temperature = Adafruit_DHT.read_retry(11, 4)

    # HOX: this fails if there is no \d+ in msp.payload
    req = re.findall('\d+', str(msg.payload))[0]
    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

    reply = '{{"Req":{},"Time":"{}","ID":"{}","T":{:0.1f},"H":{:0.1f}}}'.format(req, timestamp, id, temperature, humidity)

    reply = '{"tag":"test","number":2.0}'

    print(reply)
    client.publish(mqtt_publish_str, reply)

    if USE_MONGO:
        write_to_db(reply)

    print('end of on_message')


client.on_connect = on_connect
client.on_message = on_message
client.connect(broker_address)

try:
    client.loop_forever()
except:
    print("error")

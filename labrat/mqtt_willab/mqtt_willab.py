import requests
import pymongo
import json
import paho.mqtt.client as mqtt

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

willab_url = 'http://weather.willab.fi/weather.json'

def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_request_str)
    print("Connected to " + broker_address)

def on_message(client, userdata, msg):
    print("Received MQTT request")
    print(msg.payload)

    r = requests.get(willab_url)
    reply = r.text
    debug(reply)
    client.publish(mqtt_publish_str, reply)
    
    if USE_MONGO:
        write_to_db(reply)

def main():
    print("Launching mqtt listener...")
    
    client = mqtt.Client()

    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker_address)

    try:
        client.loop_forever()
    except:
        print("error")

    print("Exiting")

if __name__ == "__main__":
    main()
    pass

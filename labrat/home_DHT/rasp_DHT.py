import os
import datetime
import time
import json
import re

from home_DHT import mqtt_publish_str, mqtt_client
from home_DHT import init_mqtt, mqtt_listen

USE_DHT = True
try:
    import Adafruit_DHT 
except:
    USE_DHT = False

DEBUG = 1
def debug(text):
    if DEBUG:
        print(text)

''' Simple DHT measurement in reply to MQTT request '''

DEVICE_ID = "RASP_501"

def on_message(client, userdata, msg):
    """ handel the mqtt request and send reply """
    req = msg.payload.decode('UTF-8')
    print('MQTT REQUEST: ' + req)

    data = do_measure()

    data.update({"REQ": req })
    data.update({"ID": DEVICE_ID})
    debug(data)

    client.publish(mqtt_publish_str,json.dumps(data))

def do_measure():
    """ read data from sensor(s) and return as dictionary """
    debug("begin do_measure")
    H,T = (0,0)
    if USE_DHT:
        H,T = Adafruit_DHT.read_retry(11, 4)
    else:
        H,T = (666,777)

    timestamp = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")
    data = {
        "TS" : timestamp,
        "T" : T,
        "H" : H,
    }
    debug(data)
    debug("end do_measure")
    return data

if __name__ == "__main__":
    init_mqtt()
    ''' overload on_message defined in home_DHT.py '''
    mqtt_client.on_message = on_message
    
    pid = os.fork()
    if pid == 0:
        mqtt_listen()
    else:
        while True:
            data = do_measure()
            data.update({"ID": DEVICE_ID})
            mqtt_client.publish(mqtt_publish_str, json.dumps(data))
            debug(data)
            time.sleep(10*60)



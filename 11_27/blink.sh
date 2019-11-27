!#/usr/bin/bash

echo "Blinking"

while true; do
	echo "101" > /sys/class/ledclass/led01/led_attr
	sleep $1
done



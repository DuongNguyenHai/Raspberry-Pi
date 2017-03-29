#!/usr/bin/python3

import RPi.GPIO as GPIO
from time import sleep

LED = 4

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED, GPIO.OUT)	# set gpio 17 is output
 
while True:
    GPIO.output(LED, 1)
    sleep(1)            # sleep 1s
    GPIO.output(LED, 0)
    sleep(1)            # sleep 1s

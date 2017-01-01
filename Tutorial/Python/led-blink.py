#!/usr/bin/python3

import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)	# set gpio 17 is output
 
while True:
    GPIO.output(17, 1)
    sleep(1)            # sleep 1s
    GPIO.output(17, 0)
    sleep(1)            # sleep 1s

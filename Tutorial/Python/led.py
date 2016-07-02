#!/usr/bin/python3

import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
 
GPIO.output(17, 1)

# while True:
#     GPIO.output(17, 1)


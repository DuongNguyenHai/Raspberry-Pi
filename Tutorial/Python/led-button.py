#!/usr/bin/python3
import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
 
while True:
    if GPIO.input(2) == False:
        GPIO.output(17, 1)
        # GPIO.output(17, not GPIO.input(17))
        # sleep(0.3)

#!/usr/bin/python3

import smbus
from time import sleep

bus = smbus.SMBus(1)

add = 0x08

while True:    
    bus.read_byte(add)

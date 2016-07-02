#!/usr/bin/python3

import smbus
from time import sleep

bus = smbus.SMBus(1)

addr = 0x08
val = 0x10

while True:    
    bus.write_byte(addr, val)
    print(val)
    sleep(1)

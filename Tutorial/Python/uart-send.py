#!/usr/bin/python3

import time
import serial

ser = serial.Serial(
	port = '/dev/ttyAMA0',
	baudrate = 9600,
	parity = serial.PARITY_NONE,
	stopbits = serial.STOPBITS_ONE,
	bytesize = serial.EIGHTBITS,
	timeout = 1
)

print("Raspberry's sending : ")

try:
    while True:
    	ser.write(b'hehe')
    	ser.flush()
    	print("hehe")
    	time.sleep(1)
except KeyboardInterrupt:
	ser.close()


#!/usr/bin/python3

import time
import serial

ser = serial.Serial(
	port='/dev/ttyUSB0',
	baudrate = 9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1
)

counter=0

while 1:
        ser.write(str(counter).encode())
        print(counter,end="")
        time.sleep(1)
        counter +=1
        if counter > 10:
                counter=0

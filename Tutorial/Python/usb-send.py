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

print("Raspberry's sending : ")

try:
	# ham code chinh o day
    while True:
        ser.write(str(counter).encode())
        print(counter,end="")
        counter +=1
        if counter > 10:
            counter=0
        time.sleep(1)
except KeyboardInterrupt:
	ser.close()

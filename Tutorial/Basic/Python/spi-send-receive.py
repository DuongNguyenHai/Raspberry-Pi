#!/usr/bin/python3

import spidev
import time
import binascii

spi = spidev.SpiDev() # create spi object
spi.open(0, 0)# open spi port 0, device (CS) 0
spi.max_speed_hz = 5000

try:
	# ham code chinh o day
    while True:
        print("send:a,",end="")
        resp = spi.xfer2([0x61]) # transfer one byte
        c = spi.readbytes(1)
        for x in c:
        	print ("receieve:",chr(x))
        time.sleep(1) # sleep for 0.1 seconds
except KeyboardInterrupt:
	spi.close()

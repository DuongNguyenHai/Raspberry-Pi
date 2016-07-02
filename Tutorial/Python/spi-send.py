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
        print("abc")
        resp = spi.xfer2([0x61,0x62,0x63,0xA]) # transfer one byte
        time.sleep(1) # sleep for 0.1 seconds
except KeyboardInterrupt:
	spi.close()

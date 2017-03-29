/*
version 0.1 Mar 8 2017
*/

#ifndef DHT22_H
#define DHT22_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
// #include <bcm2835.h> // if use bcm2835 library

#ifndef WIRINGPI_LIB
#define WIRINGPI_LIB
#endif

#define VAL_FAIL -100			// define wrong value, its used for checking fail data

class DHT22 {

public:
	DHT22(uint8_t pin);
	~DHT22();
	bool read(float *arry);
	float tempt();
	float humidity();
private:
	uint8_t pin_;
};

#endif
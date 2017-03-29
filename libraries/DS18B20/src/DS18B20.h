/*

Author : Nguyen Hai Duong
Mar 8 2017
=========================

This is library for DS18B20 sensor. Its work on raspberry pi

*/ 

#ifndef DS18B20_H
#define DS18B20_H

#include <string.h>
#include "OneWire/OneWire.h"

#define VAL_FAIL -100			// define wrong value, its used for checking fail data

class DS18B20 : public OneWire {

public:
	DS18B20(uint8_t pin) : OneWire(pin) {}; // add pin for one wire pin
	~DS18B20();
	void setAddr(const uint8_t rom[8]);		// set address for only slave device
	float tempt();									// read temperature

private:
	uint8_t addr_[8];
};

#endif
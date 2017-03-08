

#ifndef DS18B20_H
#define DS18B20_H

#include "OneWire/OneWire.h"
#include <stdio.h>
#include <string.h>

#define VAL_FAIL -100

class DS18B20 : public OneWire {

public:
	DS18B20(uint8_t pin) : OneWire(pin) {};
	~DS18B20();
	void setAddr(const uint8_t rom[8]);
	float tempt();

private:
	uint8_t addr_[8];
};

#endif
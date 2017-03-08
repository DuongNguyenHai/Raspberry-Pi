#include "DS18B20.h"

DS18B20::~DS18B20() {}

void DS18B20::setAddr(const uint8_t rom[8]) {
	OneWire::begin();
	memcpy(addr_, rom, 8);
}

float DS18B20::tempt() {

	for(int i=0; i<7; i++) { // trying to get data in 7 seven times
	   OneWire::reset();
		OneWire::select(addr_);
		OneWire::write(0x44);
		delay(900);

		OneWire::reset();
		OneWire::select(addr_);
		OneWire::write(0xBE);
		uint8_t data[9];

		for (int i = 0; i < 9; i++) {
			data[i] = OneWire::read();
		}
		if (OneWire::crc8(data, 8) != data[8]) {
			// printf("crc is wrong, data is not valid\n");
		} else {
			return (float)((data[1] << 8) | data[0]) / 16.0;
		}

		delay(100);
	}
	
	return VAL_FAIL;

}
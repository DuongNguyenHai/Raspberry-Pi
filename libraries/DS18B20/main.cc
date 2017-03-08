// g++ -std=c++11 -Wall main.cc DS18B20.cpp OneWire/OneWire.cpp -o main -lwiringPi

#include <stdio.h>
#include "DS18B20.h"

uint8_t hot_rom[8] = {0x28, 0xff, 0xc6, 0x13, 0x50, 0x16, 0x4, 0x1a};
uint8_t cold_rom[8] = {0x28, 0xff, 0xbd, 0xb7, 0x44, 0x16, 0x3, 0xf7};
DS18B20 side_hot(17);
DS18B20 side_cold(17);

int main(int argc, char const *argv[])
{
	wiringPiSetupGpio();

	side_hot.setAddr(hot_rom);
	side_cold.setAddr(cold_rom);

	float temptHot = side_hot.tempt();
	printf("Tempt in hot side: %f\n", temptHot);
	float temptCold = side_cold.tempt();
	printf("Tempt in cold side: %f\n", temptCold);
	return 0;
}
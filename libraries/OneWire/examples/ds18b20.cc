// g++ -Wall -std=c++11 ds18b20.cc ../src/OneWire.cpp -o ds -lwiringPi

#include "../src/OneWire.h"
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <strings.h>

uint8_t addr[8];

OneWire ds(17);

int main(int argc, char const *argv[])
{
   wiringPiSetupGpio();

   if ( !ds.search(addr) ) {
      printf("No more addresses.\n");
      ds.reset_search();
   }

	for (int i = 0; i < 8; i++) {
		printf("%#0x ", addr[i]);
	}

	printf("\n");
	if ( !ds.search(addr) ) {
      printf("No more addresses.\n");
      ds.reset_search();
   }

	for (int i = 0; i < 8; i++) {
		printf("%#0x ", addr[i]);
	}

	printf("\n");
	while(1) {

		ds.reset();
		ds.select(addr);
		ds.write(0x44);
		delay(1000);

		ds.reset();
		ds.select(addr);
		ds.write(0xBE);
		uint8_t data[9];
		for (int i = 0; i < 9; i++) {
			data[i] = ds.read();
			printf("%#0x ", data[i]);
		}

		if ( ds.crc8(data, 8) != data[8]) {
			printf("crc is wrong, data is not valid\n");
		} else {
			float celsius = (float)((data[1] << 8) | data[0]) / 16.0;
			printf("celsius: %f\n", celsius);
		}

		usleep(1000);
		bzero(data, 8);
	}
   

	return 0;
}
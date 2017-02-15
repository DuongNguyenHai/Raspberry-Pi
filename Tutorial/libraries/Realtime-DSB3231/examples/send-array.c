// g++ send-array.c ../src/Wire/Wire.cpp -o send

#include "../src/Wire/Wire.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
	Wire.begin();

	uint8_t val = 10;
	uint8_t dt[8] = {0,0,0,6,0,24,1,23};
	uint8_t *p = dt;
	p++;
	while(1) {
		printf("send: %d, %d, %d, %d\n", dt[0], dt[1], dt[2], dt[3]);
		Wire.beginTransmission(0x08);
		// Wire.write(dt, 4);
		// Wire.write(10);
		Wire.write(dt, 8);

		Wire.endTransmission();
		usleep(1000000);
	}	
	return 0;
}
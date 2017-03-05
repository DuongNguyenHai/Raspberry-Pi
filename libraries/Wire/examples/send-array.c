// g++ send-array.c ../src/Wire.cpp -o send-array

#include "../src/Wire.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
	Wire.begin();
	uint8_t dt[3] = {3,4,5};
	while(1) {
		Wire.beginTransmission(0x13);
		Wire.write(1);
		Wire.write(dt, 3);
		Wire.endTransmission();
		usleep(1000000);
	}	
	return 0;
}
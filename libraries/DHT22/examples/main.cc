// g++ -Wall main.cc ../src/DHT22.cpp -o main -lwiringPi

#include <stdio.h>
#include "../src/DHT22.h"

DHT22 dht(4);

int main(int argc, char const *argv[])
{
	if (wiringPiSetupGpio() == -1)
		return -1;

	float arry[2];

	while (1) {
		// method 1
		dht.read(arry);
		printf("temperature: %.1f *C, humidity: %.1f %%\n", arry[0], arry[1]);

		// method 2
		// printf("temperature: %.1f *C, humidity: %.1f %%\n", dht.tempt(),dht.humidity());
		delay(1000);
	}

	return 0;
}


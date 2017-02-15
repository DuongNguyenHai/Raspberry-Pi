// g++ -Wall read.c ../BH1750.cpp ../Wire/Wire.cpp -o read

#include <stdio.h>
#include "../BH1750.h"

BH1750 lightMeter;

int main(){

	lightMeter.begin();
	int lux;

	while(1){
		lux = lightMeter.readLightLevel();
		printf("light: %d\n", lux);
		usleep(1000000);
	}
	return 0;
}

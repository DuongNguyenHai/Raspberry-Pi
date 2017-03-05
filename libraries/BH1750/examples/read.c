// g++ -Wall read.c ../src/BH1750.cpp ../src/Wire/Wire.cpp -o read

#include <stdio.h>
#include "../src/BH1750.h"

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

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define addrSlave 0x08

int val = 0x10;

int main(){

	int fd = wiringPiI2CSetup(addrSlave);
	
	while(1){
		int f = wiringPiI2CWrite(fd, val);
		printf("%d", f);
		fflush(stdout);
		delay(1000);
	}
	return 0;
}

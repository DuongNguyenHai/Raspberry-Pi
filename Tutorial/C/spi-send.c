#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define chanel 0
#define speed 500000
#define	SS0	8

int main(){
	unsigned char buff[6] = "abc\n";
	pinMode(SS0, OUTPUT);
	digitalWrite(SS0, 1);
	int fd;
	if( (wiringPiSPISetup(chanel, speed)) <0){
		fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
		return 0;
	}
	
	fd = wiringPiSPIGetFd (0) ;

	while(1){
		printf("%s",buff);
		fflush(stdout);
		digitalWrite(SS0, 0);
		write (fd, buff, 6) ;
		digitalWrite(SS0, 1);
		delay(1000);
	}
	return 0;
}

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
	unsigned char buff[6] = "a";
	char c[100];
	pinMode(SS0, OUTPUT);
	digitalWrite(SS0, 1);
	
	int fd;
	if( wiringPiSPISetup(chanel, speed) <0){
		fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
		return 0;
	}
	
	fd = wiringPiSPIGetFd (0) ;

	while(1){
		printf("Send:%s",buff);
		fflush(stdout);
		digitalWrite(SS0, 0);
		write (fd, buff, 1);
		digitalWrite(SS0, 1);
		delayMicroseconds(20);
		read (fd, c, 1);
		printf(",Receive:%s\n",c);
		fflush(stdout);
		
		delay(1000);
	}
	return 0;
}

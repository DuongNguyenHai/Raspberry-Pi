// Compile : gcc -Wall uart-send.c -o uart-send -lwiringPi

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {

	int fd;

	printf("Raspberry's sending : \n");
	
	while(1) {
		if((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0 ){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
		}
		serialPuts(fd, "hello");
		serialFlush(fd);
		printf("%s\n", "hello");
		fflush(stdout);
		delay(1000);
	}
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
	int fd ;
	int counter=0;
	char c[10];
	printf("Raspberry's sending : \n");
	
	while(1){
		if((fd = serialOpen ("/dev/ttyUSB0", 9600)) < 0 ){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		}
		serialPutchar(fd,counter);
		printf("%d",counter);
		fflush(stdout);
		counter++;
		if(counter>10)
			counter=0;
		delay(1000);
	}
	return 0;
}

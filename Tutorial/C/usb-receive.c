#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main(){
	int fd ;
	char c;
	while(1){
		if((fd = serialOpen ("/dev/ttyUSB0", 9600)) < 0 ){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		}else{
			do{
				c = serialGetchar(fd);
				printf("%c",c);
				fflush (stdout);
			}while(serialDataAvail(fd));
		}
	}
	return 0;
}

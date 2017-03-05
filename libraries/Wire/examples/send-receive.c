// g++ send-receive.c ../src/Wire.cpp -o send-receive

#include "../src/Wire.h"
#include <unistd.h>

#define addrSlave 0x13

int main(int argc, char const *argv[])
{
	Wire.begin();
	unsigned char buffer_tx[2] = {0x01,0x02};
	unsigned char buffer_rx[5] = {0};
	while(1) {
		printf("Write 2 bytes to arduino.\n");
		Wire.beginTransmission(addrSlave);
		Wire.write(buffer_tx, 2);
		Wire.endTransmission();
		usleep(2000000);
		Wire.requestFrom(addrSlave, 5);
		if(Wire.available()) {
			Wire.read(buffer_rx, 5);
			printf("Data read from arduino: \n");
			for(int i=0; i<5; i++) {
			   printf("-\tbuffer_rx[%d]: %#x\n", i, buffer_rx[i]);
			}
		}
		usleep(2000000);
	}	
	return 0;
}
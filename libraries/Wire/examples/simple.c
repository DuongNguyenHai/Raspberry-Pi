#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


int main() {

	int file_i2c;
	int length;
	unsigned char buffer_tx[2] = {0};
	unsigned char buffer_rx[5] = {0};
	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus");
		return -1;
	}
	
	// Add address if I2C slave
	int addr = 0x13; // address of i2c slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return -1;
	}
	
	buffer_tx[0] = 0x01;
	buffer_tx[1] = 0x02;

	while(1){

		//----- WRITE BYTES -----
	   length = 2;														// Number of bytes to write
		if (write(file_i2c, buffer_tx, length) != length)		// write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
		{
			printf("Failed to write to the i2c bus.\n");
		} else {
			printf("Write %d bytes to arduino.\n", length);
		}
		usleep(2000000);

		//----- READ BYTES -----
		length = 5;														// Number of bytes to read
		if (read(file_i2c, buffer_rx, length) != length)	// read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
		{
			printf("Failed to read from the i2c bus.\n");
		} else {
			printf("Data read from arduino: \n");
			for(int i=0; i<length; i++) {
			   printf("-\tbuffer_rx[%d]: %#x\n", i, buffer_rx[i]);
			}
		}

		usleep(2000000);

	}
	
	return 0;
}
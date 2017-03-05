#include "Wire.h"

TwoWire::TwoWire() {}

TwoWire::~TwoWire() {
	close(fd_);
}

bool TwoWire::begin(char *path) {
	if ((fd_ = open(path, O_RDWR)) == -1)
		return false;
	else {
		transmitting_ = false;
		return true;
	}
}

int8_t TwoWire::beginTransmission(uint16_t addr) {
	if (ioctl(fd_, I2C_SLAVE, addr) == -1)
		return false;
	else {
		transmitting_ = true;
		addr_ = addr;
		return true;
	}
}

int8_t TwoWire::endTransmission() {

	int ret = pwrite(fd_, txBuffer, txBufferLength, 0);

	transmitting_ = false;
	addr_ = 0;
	txBufferLength = 0;
	return ret;
}

int8_t TwoWire::requestFrom(uint16_t addr, uint8_t length) {

	if (ioctl(fd_, I2C_SLAVE, addr) == -1)
		return -1;

	if(length>I2C_SMBUS_BLOCK_MAX) 
		length = I2C_SMBUS_BLOCK_MAX;
		
	int8_t ret = pread(fd_, rxBuffer, length, 0);
	rxBufferIndex = 0;
	rxBufferLength = length;
	
	return ret;
}

uint8_t TwoWire::available() {
	return rxBufferLength - rxBufferIndex;
}

uint8_t TwoWire::write(int8_t dt) {
	if(txBufferLength<I2C_SMBUS_BLOCK_MAX) {
		txBuffer[txBufferLength] = dt;
		txBufferLength++;
		return 1;
	}
	return 0;
}

uint8_t TwoWire::write(const uint8_t *dt, uint8_t length) {
	uint8_t n = 0;

	if(length>I2C_SMBUS_BLOCK_MAX) 
		length = I2C_SMBUS_BLOCK_MAX;

	for(int i=0; i<length; i++){
	   n += write(dt[i]);
	}

	return n;
}

int16_t TwoWire::read() {
	if(rxBufferIndex < rxBufferLength){
		rxBufferIndex++;
		return rxBuffer[rxBufferIndex-1];
	} else
		return -1;
}


uint8_t TwoWire::read(uint8_t *dt, uint8_t length) {
	if (length > rxBufferLength)
		length = rxBufferLength;

	for(int i=0; i<length; i++){
	   dt[i] = rxBuffer[i];
	}

	return length;
}

TwoWire Wire;
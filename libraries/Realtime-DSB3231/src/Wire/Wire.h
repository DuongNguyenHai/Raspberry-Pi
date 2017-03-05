// Author : Nguyen Hai Duong, Feb 14 2017
// 

#ifndef TwoWire_h
#define TwoWire_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define pathI2C_0 (char*)"/dev/i2c-0"
#define pathI2C_1 (char*)"/dev/i2c-1"


// I2C definitions
#define I2C_SLAVE	0x0703
#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */	

class TwoWire {

public: 
	TwoWire();
	~TwoWire();
	bool begin(char *path = pathI2C_1);
	int8_t beginTransmission(uint16_t addr);
   int8_t endTransmission(void);
   int8_t requestFrom(uint16_t addr, uint8_t length);
   uint8_t available(void);
	uint8_t write(int8_t dt);
	uint8_t write(const uint8_t *dt, uint8_t length);
	int16_t read();
	uint8_t read(uint8_t *dt, uint8_t length);
	uint16_t fd_;
private:
	uint16_t addr_;
	
	bool transmitting_;
	uint8_t rxBuffer[I2C_SMBUS_BLOCK_MAX];
	uint8_t rxBufferIndex;
	uint8_t rxBufferLength;

	uint8_t txBuffer[I2C_SMBUS_BLOCK_MAX];
	uint8_t txBufferLength;

};

extern TwoWire Wire;

#endif
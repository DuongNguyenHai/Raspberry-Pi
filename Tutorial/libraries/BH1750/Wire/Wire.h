// Author : Nguyen Hai Duong, Feb 14 2017
// 

#ifndef TwoWire_h
#define TwoWire_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define pathI2C (char*)"/dev/i2c-1"


// I2C definitions

#define I2C_SLAVE	0x0703
#define I2C_SMBUS	0x0720	/* SMBus-level access */

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0
#define I2C_RDWR	0x0707	/* Combined R/W transfer (one stop only)*/
// SMBus transaction types

#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages

#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */	
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */

class TwoWire {

public: 
	TwoWire();
	~TwoWire();
	bool begin(char *path = pathI2C);
	bool beginTransmission(uint16_t addr);
   void endTransmission(void);
   int8_t requestFrom(uint16_t addr, uint8_t length);
   int8_t requestFrom(uint16_t addr, uint8_t length, uint8_t command);
   uint8_t available(void);
	int8_t write(int8_t dt);
	int8_t write(const uint8_t *dt, uint8_t length);
	int8_t write(const uint8_t *dt, uint8_t command, uint8_t length);
	int16_t read();
	uint8_t read(uint8_t *dt, uint8_t length);
	
private:
	uint16_t addr_;
	uint16_t fd_;
	bool transmitting_;
	uint8_t rxBuffer[I2C_SMBUS_BLOCK_MAX];
	uint8_t rxBufferIndex;
	uint8_t rxBufferLength;

};

extern TwoWire Wire;

#endif
#include "Wire.h"

struct i2c_msg {
	uint16_t addr;	/* slave address			*/
	unsigned short flags;		
#define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
#define I2C_M_RD	0x01
#define I2C_M_NOSTART	0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
	uint8_t len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

// Structures used in the ioctl() calls
union i2c_smbus_data
{
  uint8_t  byte;
  uint16_t word;
  uint8_t  block [I2C_SMBUS_BLOCK_MAX];	// block [0] is used for length + one more for PEC
};

struct i2c_smbus_ioctl_data
{
  char read_write;
  uint8_t command;
  int size;
  union i2c_smbus_data *data;
};

struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/* pointers to i2c_msgs */
	int nmsgs;		/* number of i2c_msgs */
};

static inline int i2c_smbus_access(int fd, char read_write, uint8_t command, 
												int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(fd, I2C_SMBUS, &args);
}

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

bool TwoWire::beginTransmission(uint16_t addr) {
	if (ioctl(fd_, I2C_SLAVE, addr) == -1)
		return false;
	else {
		transmitting_ = true;
		addr_ = addr;
		return true;
	}
}

void TwoWire::endTransmission() {
	transmitting_ = false;
	addr_ = 0;
}

int8_t TwoWire::requestFrom(uint16_t addr, uint8_t length) {

	if (ioctl(fd_, I2C_SLAVE, addr) == -1)
		return -1;

	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg msg;

	msg.addr = addr;
	msg.flags = I2C_SMBUS_READ;
	msg.len = length;
	msg.buf = rxBuffer;

	packets.msgs = &msg;
	packets.nmsgs = 1;

	if(ioctl(fd_, I2C_RDWR, &packets)) {
		rxBufferIndex = 0;
		rxBufferLength = length;
		return 0;
	} else
		return -1;
}

int8_t TwoWire::requestFrom(uint16_t addr, uint8_t command, uint8_t length) {
	if (ioctl(fd_, I2C_SLAVE, addr) == -1)
		return -1;

	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;

	rxBufferIndex = 0;
	
	union i2c_smbus_data data;
	if (i2c_smbus_access(fd_, I2C_SMBUS_READ, command, length+1, &data))
		return -1;
	else {
		for (uint8_t i = 0; i < length; i++) {
			rxBuffer[i] = data.block[i];
		}
		rxBufferLength = length;
	}

	return 0;
}

uint8_t TwoWire::available() {
	return rxBufferLength - rxBufferIndex;
}

int8_t TwoWire::write(int8_t dt) {
	if(transmitting_) {
		return i2c_smbus_access(fd_, I2C_SMBUS_WRITE, dt,
	                        I2C_SMBUS_BYTE, NULL);
	}
	else return -1;
}

int8_t TwoWire::write(const uint8_t *dt, uint8_t length) {
	if(transmitting_) {
		struct i2c_rdwr_ioctl_data packets;
		struct i2c_msg msg;

		msg.addr = addr_;
		msg.flags = I2C_SMBUS_WRITE;
		msg.len = length;
		msg.buf = (uint8_t *)dt;

		packets.msgs = &msg;
		packets.nmsgs = 1;

		return ioctl(fd_, I2C_RDWR, &packets);
	} else
		return -1;
	
}

int8_t TwoWire::write(const uint8_t *dt, uint8_t command, uint8_t length) {
	if(transmitting_) {

		union i2c_smbus_data data;

		if (length > I2C_SMBUS_BLOCK_MAX)
			length = I2C_SMBUS_BLOCK_MAX;

		for (uint8_t i = 0; i <= length; i++)
			data.block[i] = dt[i];

		return i2c_smbus_access(fd_, I2C_SMBUS_WRITE, command,
		                        I2C_SMBUS_BLOCK_DATA, &data);
	} else
		return -1;
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
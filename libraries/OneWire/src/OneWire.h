/*

Fixer : Nguyen Hai Duong
Mar 8 2017
=========================

Version 0.1 Mar 8 2017

ALl of function name and its functionality was copied from One Wire library for arduino and
it has been maintained by Paul Stoffregen (paul@pjrc.com) since January 2010. But, the content
inside functions are different.

I just call my self is "Fixer" because all of things I've done were copying and fixing.
U can find onthers libraries for Raspberry Pi at 
https://github.com/DuongNguyenHai/Raspberry-Pi/tree/master/libraries

The MIT License

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef OneWire_h
#define OneWire_h

#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
// #include <bcm2835.h> // if use bcm2835 library

#ifndef WIRINGPI_LIB
#define WIRINGPI_LIB
#endif


class OneWire {

public:
	OneWire(uint8_t pin);
	~OneWire();
	void begin();
	// Perform a 1-Wire reset cycle. Returns 1 if a device responds
	// with a presence pulse.  Returns 0 if there is no device or the
	// bus is shorted or otherwise held low for more than 250uS
	uint8_t reset(void);

	// Issue a 1-Wire rom select command, you do the reset first.
	void select(const uint8_t rom[8]);

	// Issue a 1-Wire rom skip command, to address all on bus.
	void skip(void);

	// Write a byte. If 'power' is one then the wire is held high at
	// the end for parasitically powered devices. You are responsible
	// for eventually depowering it by calling depower() or doing
	// another read or write.
	// The two function below has not supported yet
	// void write(uint8_t v, uint8_t power = 0);

	// void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);

	void write(uint8_t val);

	void write_bytes(const uint8_t *buf, uint16_t count);

	// Read a byte.
	uint8_t read(void);

	void read_bytes(uint8_t *buf, uint16_t count);

	// Write a bit. The bus is always left powered at the end, see
	// note in write() about that.
	void write_bit(uint8_t v);

	// Read a bit.
	uint8_t read_bit(void);

	// Stop forcing power onto the bus. You only need to do this if
	// you used the 'power' flag to write() or used a write_bit() call
	// and aren't about to do another read or write. You would rather
	// not leave this powered if you don't have to, just in case
	// someone shorts your bus.
	void depower(void);

	// Clear the search state so that if will start from the beginning again.
	void reset_search();

	// Setup the search to find the device type 'family_code' on the next call
	// to search(*newAddr) if it is present.
	void target_search(uint8_t family_code);

	// Look for the next device. Returns 1 if a new address has been
	// returned. A zero might mean that the bus is shorted, there are
	// no devices, or you have already retrieved all of them.  It
	// might be a good idea to check the CRC to make sure you didn't
	// get garbage.  The order is deterministic. You will always get
	// the same devices in the same order.
	uint8_t search(uint8_t *newAddr);

	static uint8_t crc8(const uint8_t *addr, uint8_t len);
	
private:
	uint8_t pin_;
	unsigned char ROM_NO[8];
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	uint8_t LastDeviceFlag;
};

#endif
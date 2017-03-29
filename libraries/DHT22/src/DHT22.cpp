#include "DHT22.h"

// define for WiringPi library
#ifdef WIRINGPI_LIB
#define pinMode_(pin, mode) pinMode(pin, mode)
#define pinWrite_(pin, level) digitalWrite(pin, level)
#define pinRead_(pin) digitalRead(pin)
#define delayUs_(time) delayMicroseconds(time)
#define delayMs_(time) delay(time)
#define OUTPUT_ OUTPUT
#define INPUT_ INPUT

#else // define below for BCM2835 library
#define pinMode_(pin, mode) bcm2835_gpio_fsel(pin, mode)
#define pinWrite_(pin, level) bcm2835_gpio_write(pin, level)
#define pinRead_(pin) bcm2835_gpio_lev(pin)
#define delayUs_(time) bcm2835_delayMicroseconds(time)
#define delayMs_(time) bcm2835_delay(time)
#define OUTPUT_ BCM2835_GPIO_FSEL_OUTP
#define INPUT_ BCM2835_GPIO_FSEL_INPT
#endif

#define MAX_TIMINGS	85
static bool readData(uint8_t pin,float *arry) {
	int data[5] = { 0 };
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0;
	/* pull pin down for 18 milliseconds */
	pinMode_(pin, OUTPUT_);
	pinWrite_(pin, LOW);
	delayMs_(18);

	/* prepare to read the pin */
	pinMode_(pin, INPUT_);

	/* detect change and read data */
	for (int i = 0; i < MAX_TIMINGS; i++)
	{
		counter = 0;
		while ( pinRead_(pin) == laststate ) {
			counter++;
			delayUs_(1);
			if ( counter == 255 )
				break;
		}
		laststate = pinRead_(pin);
		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) ) {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) && (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) ) {
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
			h = data[0];	// for DHT11
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
			c = data[2];	// for DHT11
		if ( data[2] & 0x80 )
			c = -c;

		arry[0] = c;
		arry[1] = h;
		return true;
	}else  {
		// printf( "Data not good, skip\n" );
		return false;
	}

}

DHT22::DHT22(uint8_t pin) {
	pin_ = pin;
};

DHT22::~DHT22() {};

bool DHT22::read(float *arry) {
	for (int i = 0; i < 15; ++i) {
		delayMs_(300);
		if(readData(pin_, arry))
			return true;
	}
	return false;
}

float DHT22::tempt() {
	float data[2];
	if(read(data))
		return data[0];
	else
		return VAL_FAIL;
}

float DHT22::humidity() {
	float data[2];
	if(read(data))
		return data[1];
	else
		return VAL_FAIL;
}
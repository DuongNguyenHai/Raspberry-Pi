#include "OneWire.h"

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

OneWire::OneWire(uint8_t pin) {
	pin_ = pin;
  reset_search();
}

OneWire::~OneWire() {}

// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//
uint8_t OneWire::reset(void) {
	pinMode_(pin_, OUTPUT_);
	pinWrite_(pin_, LOW);
	delayUs_(480);
	pinMode_(pin_, INPUT_);
	delayUs_(70);
	uint8_t b = !pinRead_(pin_);
	delayUs_(410);
	return b;
}

// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
void OneWire::write_bit(uint8_t v) {  
	int delay_set, delay_release;
	if (v == 1) {
		delay_set = 10;
		delay_release = 55;
	} else {
		delay_set = 60;
		delay_release = 10;
	}
	pinMode_(pin_, OUTPUT_);
	pinWrite_(pin_, LOW);
	delayUs_(delay_set);
	pinMode_(pin_, INPUT_);
	delayUs_(delay_release);
}

// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
uint8_t OneWire::read_bit() {
	pinMode_(pin_, OUTPUT_);
	pinWrite_(pin_, LOW);
	delayUs_(6);
	pinMode_(pin_, INPUT_);
	delayUs_(9);
	uint8_t b = pinRead_(pin_);
	delayUs_(55);

	return b;
}

// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//
void OneWire::write(uint8_t val) {
  int i;
  for (i = 0; i < 8; i++) {
    if (val & 1)
      write_bit(1);
    else
      write_bit(0);
    
    val = val >> 1;
  }
}

void OneWire::write_bytes(const uint8_t *buf, uint16_t count) {
  for (uint16_t i = 0 ; i < count ; i++)
    write(buf[i]);
}

// Read a byte
//
uint8_t OneWire::read() {
	uint8_t val = 0;
	for (int i = 0; i < 8; i++) {
	 val = val | read_bit() << i;
	}
	return val;
}

void OneWire::read_bytes(uint8_t *buf, uint16_t count) {
	for (uint16_t i = 0 ; i < count ; i++)
	 buf[i] = read();
}

// Do a ROM select
//
void OneWire::select(const uint8_t rom[8]) {
	uint8_t i;
	write(0x55);           // Choose ROM
	for (i = 0; i < 8; i++) write(rom[i]);
}

// Do a ROM skip
//
void OneWire::skip() {
	write(0xCC);           // Skip ROM
}

//
// You need to use this function to start a search again from the beginning.
// You do not need to do it for the first search, though you could.
//
void OneWire::reset_search()
{
  // reset the search state
  LastDiscrepancy = 0;
  LastDeviceFlag = false;
  LastFamilyDiscrepancy = 0;
  for(int i = 7; ; i--) {
    ROM_NO[i] = 0;
    if ( i == 0) break;
  }
}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
void OneWire::target_search(uint8_t family_code)
{
   // set the search state to find SearchFamily type devices
   ROM_NO[0] = family_code;
   for (uint8_t i = 1; i < 8; i++)
      ROM_NO[i] = 0;
   LastDiscrepancy = 64;
   LastFamilyDiscrepancy = 0;
   LastDeviceFlag = false;
}

// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return true  : device found, ROM number in ROM_NO buffer
//        false : device not found, end of search
//
uint8_t OneWire::search(uint8_t *newAddr) {
   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number, search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!reset())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = false;
         LastFamilyDiscrepancy = 0;
         return false;
      }

      // issue the search command
      write(0xF0);

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = read_bit();
         cmp_id_bit = read_bit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            write_bit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
            LastDeviceFlag = true;

         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      search_result = false;
   }
   for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
   return search_result;
}

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
uint8_t OneWire::crc8(const uint8_t *addr, uint8_t len) {
	uint8_t crc = 0;
	
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
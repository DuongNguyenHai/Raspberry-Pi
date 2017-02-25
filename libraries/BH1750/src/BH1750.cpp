#include "BH1750.h"

#define delay_ms_(time_ms) usleep(time_ms*1000);

BH1750::BH1750(unsigned char addr) {
   BH1750_I2CADDR = addr;
}

void BH1750::begin(unsigned char mode) {
   Wire.begin();
   configure(mode);
}

void BH1750::configure(uint8_t mode) {

   // Check, is measurment mode exist
   switch (mode) {

      case BH1750_CONTINUOUS_HIGH_RES_MODE: {
         // Send mode to sensor
         Wire.beginTransmission(BH1750_I2CADDR);
         Wire.write((uint8_t)mode);
         Wire.endTransmission();

         // Wait few moments for waking up
         delay_ms_(100);
         
      } break;
      case BH1750_CONTINUOUS_HIGH_RES_MODE_2:
      case BH1750_CONTINUOUS_LOW_RES_MODE:
      case BH1750_ONE_TIME_HIGH_RES_MODE:
      case BH1750_ONE_TIME_HIGH_RES_MODE_2:
      case BH1750_ONE_TIME_LOW_RES_MODE:
      default: // Invalid measurement mode
         break;

   }

}

uint16_t BH1750::readLightLevel(void) {

   // Measurment result will be stored here
   uint16_t level;

   // Read two bytes from sensor
   Wire.requestFrom(BH1750_I2CADDR, 2);

   // Read two bytes, which are low and high parts of sensor value
   level = Wire.read();
   level <<= 8;
   level |= Wire.read();

   // Convert raw value to lux
   level /= 1.2;

   return level;

}
#include <SPI.h>

#define SS 10

void setup() {
   pinMode(SS, OUTPUT);
   // SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
   SPI.begin();
   SPI.setBitOrder(MSBFIRST);
   SPI.setClockDivider(SPI_CLOCK_DIV4);
   SPI.setDataMode(SPI_MODE0);

}

void loop() {

   for (char a=0; a<256; a++) {
      digitalWrite(SS, LOW);
      SPI.transfer(a); // send value (0~255)
      digitalWrite(SS, HIGH);
      delay(1000);
   }

}

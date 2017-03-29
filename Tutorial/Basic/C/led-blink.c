// gcc -Wall led-blink.c -o led-blink -lwiringPi

#include <wiringPi.h>

#define LED 4

int main(void)
{
    wiringPiSetupGpio();
    pinMode(LED, OUTPUT);
    
    while(1)
    {
        digitalWrite(LED, HIGH); 
        delay(1000);            // delay 1000 ms
        digitalWrite(LED, LOW);
        delay(1000);            // delay 1000 ms
    }
    return 0;
}

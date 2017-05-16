// gcc -Wall led.c -o led -lwiringPi

#include <wiringPi.h>

#define LED 17

int main(void)
{
    wiringPiSetupGpio();
    pinMode(LED, OUTPUT);
    
    while(1) {
        digitalWrite(LED, HIGH); 
    }
    return 0;
}

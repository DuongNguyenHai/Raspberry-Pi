// gcc -Wall led.c -o led -lwiringPi

#include <wiringPi.h>

int main(void)
{
    wiringPiSetupGpio();
    pinMode(17, OUTPUT);
    
    while(1) {
        digitalWrite(17, HIGH); 
    }
    return 0;
}

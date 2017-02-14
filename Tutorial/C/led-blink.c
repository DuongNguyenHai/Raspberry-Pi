// gcc -Wall led-blink.c -o led-blink -lwiringPi

#include <wiringPi.h>

int main(void)
{
    wiringPiSetupGpio();
    pinMode(17, OUTPUT);
    
    while(1)
    {
        digitalWrite(17, HIGH); 
        delay(1000);            // delay 1000 ms
        digitalWrite(17, LOW);
        delay(1000);            // delay 1000 ms
    }
    return 0;
}

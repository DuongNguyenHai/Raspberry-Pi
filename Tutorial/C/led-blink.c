#include <wiringPi.h>

int main(void)
{
    wiringPiSetupGPIO();
    pinMode(17, OUTPUT);
    
    while(1)
    {
        digitalWrite(17, HIGH); 
        delay(1000);
        digitalWrite(17, LOW);
        delay(1000);
    }
    return 0;
}

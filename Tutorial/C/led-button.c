#include <wiringPi.h>

int main(void)
{
    wiringPiSetupGPIO();
    pinMode(17, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl (2, PUD_UP);
    
    digitalWrite(17, 0); 
    while(1)
    {
        if(!digitalRead(2)){
            digitalWrite(17, 1);
            // digitalWrite(17, !digitalRead(17));
            delay(300);
        }
    }
    
    return 0;
}

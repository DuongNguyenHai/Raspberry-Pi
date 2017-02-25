// g++ -Wall time.c ../src/RTClib.cpp ../src/Wire/Wire.cpp -o time -lwiringPi

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../src/RTClib.h"

#define ALARM_PIN 17
RTC_DS3231 rtc;


int main(int argc, char const *argv[])
{
	char t[22];
   wiringPiSetupGpio();
   pinMode(ALARM_PIN, INPUT);

	if (! rtc.begin()) {
      printf("Couldn't find RTC\n");
      while (1);
   }

   DateTime dt(2017, 01, 18, 05, 59, 55);

   printf("set: %d/%d/%dT%d:%s%d:%s%d\n", dt.year(), dt.month(), dt.day(), dt.hour(), 
   			((dt.minute() > 9) ? "": "0"), dt.minute(), ((dt.second() > 9) ? "": "0"), dt.second());

 	rtc.adjust(dt);

   alarm_t alarmTime;

   alarmTime.d = 0;
   alarmTime.hh = 5;
   alarmTime.mm = 59;
   alarmTime.ss = 58;

   rtc.setAlarm(ALARM_1, alarmTime, MATCH_HHMMSS_OR_HHMM);
   printf("set alarm 1 at %d:%d:%d\n", alarmTime.hh, alarmTime.mm, alarmTime.ss);
   rtc.setAlarmHour(ALARM_2, 6);
   printf("set alarm 2 at 6h\n");

   if (rtc.lostPower()) {
      printf("RTC lost power, lets set the time!\n");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(__DATE__, __TIME__));
      // rtc.adjust(DateTime(2017, 01, 18, 06, 00, 00));
   }

	while(1) {
		rtc.getDateTimeString(t);
		printf("Date time: %s\n", t);
      if(digitalRead(ALARM_PIN)==LOW) {
         if(rtc.isAlarmRing(ALARM_1)) {
            printf("alarm 1 is on !\n");
            rtc.clearAlarm(ALARM_1);
         }
         if(rtc.isAlarmRing(ALARM_2)) {
            printf("alarm 2 is on !\n");
            rtc.clearAlarm(ALARM_2);
         }
      }
		usleep(1000000);
	}

	return 0;
}
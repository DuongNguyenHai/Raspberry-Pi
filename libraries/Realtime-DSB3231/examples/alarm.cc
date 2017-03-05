// g++ -Wall -std=c++11 alarm.cc ../src/RTClib.cpp ../src/Wire/Wire.cpp -o alarm

#include <stdio.h>
#include "../src/RTClib.h"

RTC_DS3231 rtc;

int main(int argc, char const *argv[])
{
   char tm[22];

   if (! rtc.begin()) {
      printf("Couldn't find RTC\n");
      while (1);
   }

   DateTime dt(2017, 01, 18, 05, 59, 55);

   printf("set: %d/%d/%dT%d:%s%d:%s%d\n", dt.year(), dt.month(), dt.day(), dt.hour(), 
            ((dt.minute() > 9) ? "": "0"), dt.minute(), ((dt.second() > 9) ? "": "0"), dt.second());

   // following line sets the RTC to the date & time.

   rtc.adjust(dt);

   alarm_t alarmTime;

   alarmTime.hh = 5;    // hour
   alarmTime.mm = 59;   // minute
   alarmTime.ss = 58;   // second

   rtc.setAlarm(ALARM_1, alarmTime, MATCH_HHMMSS_OR_HHMM);  // match hour + minute + second with alarm 1
   printf("set alarm 1 at %d:%d:%d\n", alarmTime.hh, alarmTime.mm, alarmTime.ss);
   rtc.setAlarmHour(ALARM_2, 6); // match hour
   printf("set alarm 2 at 6h\n");

   while(1) {
      rtc.getDateTimeString(tm);
      printf("Date time: %s\n", tm);
      if(rtc.isAlarmRinging(ALARM_1)) {
         printf("alarm 1 is on !\n");
         rtc.clearAlarm(ALARM_1);
      }
      if(rtc.isAlarmRinging(ALARM_2)) {
         printf("alarm 2 is on !\n");
         rtc.clearAlarm(ALARM_2);
      }
      usleep(1000000);
   }

   return 0;
}